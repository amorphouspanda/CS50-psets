import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Ensure environment variable is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    row = db.execute("SELECT cash FROM users WHERE id = :id",
                     id=session["user_id"])

    cash = row[0]["cash"]

    history = db.execute("SELECT * FROM buys WHERE user = :user",
                         user=session["user_id"])

    stocks = []
    stocksWorth = 0

    for a in range(len(history)):
        symbol = history[a]["symbol"]
        shares = history[a]["shares"]

        quote = lookup(symbol)
        price = quote["price"]

        total = shares * price
        stocksWorth += total

        owned = False
        for b in range(a):
            if (symbol == history[b]["symbol"]):
                stocks[b][2] = stocks[b][2] + shares
                stocks[b][3] = stocks[b][3] + total
                stocks[b][3] = stocks[b][3]

                owned = True
                break

        if not owned:
            stocks.append([
                symbol,
                price,
                shares,
                total
            ])

    display = []

    for a in range(len(stocks)):
        if (stocks[a][2] == 0):
            continue

        display.append([
            stocks[a][0],
            usd(stocks[a][1]),
            stocks[a][2],
            usd(stocks[a][3])
        ])

    netWorth = cash + stocksWorth

    return render_template("index.html", stocks=display, cash=usd(cash), netWorth=usd(netWorth))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":
        quote = lookup(request.form.get("symbol"))

        if quote == None:
            return apology("please enter a valid symbol", 400)

        symbol = quote["symbol"]
        price = quote["price"]

        try:
            shares = int(request.form.get("shares"))
        except:
            return apology("please enter a positive integer", 400)

        if shares <= 0:
            return apology("please enter a positive integer", 400)

        cash = db.execute("SELECT cash FROM users WHERE id = :id",
                          id=session["user_id"])

        cashRemaining = cash[0]["cash"] - (price * shares)

        if cashRemaining < 0:
            return apology("you don't have enough cash")

        db.execute("UPDATE users SET cash = :cashRemaining WHERE id = :id",
                   cashRemaining=cashRemaining, id=session["user_id"])

        db.execute("INSERT INTO buys (user, symbol, price, shares) VALUES(:user, :symbol, :price, :shares)",
                   user=session["user_id"], symbol=symbol, price=price, shares=shares)

        flash("Transaction completed")

        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    history = db.execute("SELECT * FROM buys WHERE user= :user",
                         user=session["user_id"])

    display = []

    for a in range(len(history)):
        if history[a]["shares"] < 0:
            action = "Sell"
        else:
            action = "Buy"

        display.append([
            history[a]["symbol"],
            usd(history[a]["price"]),
            abs(history[a]["shares"]),
            action,
            history[a]["time"]
        ])

    return render_template("history.html", history=display)


@app.route("/deposit", methods=["GET", "POST"])
@login_required
def deposit():
    """Deposit cash into account"""

    if request.method == "POST":
        deposit = int(request.form.get("deposit"))

        if deposit <= 0:
            return apology("please enter a positive number", 400)

        buffer = db.execute("SELECT cash FROM users WHERE id = :id",
                            id=session["user_id"])

        cash = buffer[0]["cash"]

        db.execute("UPDATE users SET cash = :cash WHERE id = :id",
                   cash=cash + deposit, id=session["user_id"])

        return redirect("/")

    else:
        return render_template("deposit.html")


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "POST":
        quote = lookup(request.form.get("symbol"))

        if quote == None:
            return apology("please enter a valid symbol", 400)

        return render_template("quoted.html", symbol=quote["symbol"], price=usd(quote["price"]))

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    session.clear()

    if request.method == "POST":
        if not request.form.get("username"):
            return apology("must provide username", 400)

        elif not request.form.get("password"):
            return apology("must provide password", 400)

        elif not request.form.get("confirmation"):
            return apology("must provide confirmation", 400)

        elif not (request.form.get("password") == request.form.get("confirmation")):
            return apology("passwords must match", 400)

        result = db.execute("INSERT INTO users (username, hash) VALUES(:username, :hash)",
                            username=request.form.get("username"), hash=generate_password_hash(request.form.get("password")))

        if not result:
            return apology("username already exists", 400)

        id = db.execute("SELECT id FROM users WHERE username IS :username",
                        username=request.form.get("username"))

        session["user_id"] = id[0]["id"]

        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "POST":
        quote = lookup(request.form.get("symbol"))

        if quote == None:
            return apology("please enter a valid symbol", 400)

        symbol = quote["symbol"]
        price = quote["price"]

        try:
            shares = int(request.form.get("shares"))
        except:
            return apology("please enter a positive integer", 400)

        if shares <= 0:
            return apology("please enter a positive integer", 400)

        ownedRows = db.execute("SELECT shares FROM buys WHERE symbol = :symbol AND user = :user",
                               symbol=symbol, user=session["user_id"])

        owned = 0
        for a in range(len(ownedRows)):
            owned += ownedRows[a]["shares"]

        if owned < shares:
            return apology("You don't own enough shares", 400)

        owned -= shares

        cash = db.execute("SELECT cash FROM users WHERE id = :id",
                          id=session["user_id"])

        cashUpdated = cash[0]["cash"] + (price * shares)

        db.execute("UPDATE users SET cash = :cashUpdated WHERE id = :id",
                   cashUpdated=cashUpdated, id=session["user_id"])

        db.execute("INSERT INTO buys (user, symbol, price, shares) VALUES(:user, :symbol, :price, :shares)",
                   user=session["user_id"], symbol=symbol, price=price, shares=(-1) * shares)

        flash("Transaction completed")

        return redirect("/")
    else:
        rows = db.execute("SELECT symbol FROM buys WHERE user = :user",
                          user=session["user_id"])

        symbols = []

        for a in range(len(rows)):
            if rows[a]["symbol"] not in symbols:
                symbols.append(rows[a]["symbol"])

        return render_template("sell.html", symbols=symbols)


def errorhandler(e):
    """Handle error"""
    return apology(e.name, e.code)


# listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
