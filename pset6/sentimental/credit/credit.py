import math

def main():
    number = input("Number: ")

    # if (not checksum(number)):
        # print("INVALID")
        # return 0

    card = checkCard(number)
    print(card)

    return 0

def checkCard(number):
    if ((number[1] == '4' or number[1] == '7') and len(number) == 15 and number[0] == '3'):
        return "AMEX"

    if (len(number) == 16 and number[0] == '5' and (int(number[1]) < 6) and number[1] != '0'):
        return "MASTERCARD"

    if ((len(number) == 13 or len(number) == 16) and number[0] == '4'):
        return "VISA"

    return "INVALID"

def checksum(number):
    sum = 0

    for num in range(1, len(number), 2):
        sum += addDigits(int(number[num]) * 2)

    for num in range(0, len(number), 2):
        sum += int(number[num])

    return sum % 10 == 0

def addDigits(num):
    sum = 0

    while num != 0:
        sum += num % 10
        num = math.floor(num /10)

    return sum

if __name__ == "__main__":
    main()
