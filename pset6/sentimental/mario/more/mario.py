
while True:
    height = int(input("Height: "))

    if (height > 0 and height < 24):
        break


for y in range(0, height):
    for space in range(0, height - (y + 1)):
        print(" ", end="")

    for x in range(0, y + 1):
        print("#", end="")

    print(" ", end="")
    print(" ", end="")

    for x in range(0, y + 1):
        print("#", end="")

    print()

