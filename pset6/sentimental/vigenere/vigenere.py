import sys

def main():
    if (len(sys.argv) != 2):
        # print("input 2 command line arguments")
        exit(1)

    alpha = True
    for k in range(len(sys.argv[1])):
        if not sys.argv[1].isalpha():
            alpha = False;

    if (not alpha):
        # print("key is not alphabetical")
        exit(1)

    text = input("plaintext: ");

    list = cipher(text, sys.argv[1])

    print("ciphertext: ", end="")

    for a in range(len(list)):
        print(list[a], end="")

    print()

    exit(0)

def cipher(text, key):
    list = []
    k = 0

    for p in range(len(text)):

        if (k + 1 > len(key)):
            k = 0

        c = 0

        if (text[p].isalpha()):
            if text[p].isupper():
                c = (((ord(text[p])) - 65) + (ord(key[k].upper()) - 65)) % 26
                c += 65
                list.append(chr(c))
            else:
                c = (((ord(text[p])) - 97) + (ord(key[k].upper()) - 65)) % 26
                c += 97
                list.append(chr(c))

        else:
            list.append(text[p])
            k -= 1

        k += 1

    return list

if __name__ == "__main__":
    main()

