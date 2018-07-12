#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int cipher(string plainText, string key);

int main(int argc, string argv[])           // Command line: gets key
{
    string key = argv[1];

    if (argc != 2)                  // Checks if key is invalid
    {
        printf("Keyword is invalidddddd\n");

        return 1;
    }
    else
    {
        for (int k = 0, n = strlen(key); k < n; k++)
        {
            if (!isalpha(key[k]))
            {
                printf("Keyword is invaliddddddddddd\n");

                return 1;
            }
        }
    }

    string plainText = get_string("plaintext: ");         // gets plaintext

    printf("ciphertext: ");
    cipher(plainText, key);
    printf("\n");

    return 0;
}

int cipher(string plainText, string key)         // turns plaintext into ciphertext
{
    for (int p = 0, k = 0, n = strlen(plainText); p < n; p++, k++)
    {
        if (k + 1 > strlen(key))          // loops key if plaintext is longer
        {
            k = 0;
        }

        int c = 0;

        if (isalpha(plainText[p]))          // prints alphabetic characters
        {
            if (isupper(plainText[p]))      // sets uppercase characters to proper numbers and converts to ciphertext
            {
                c = ((plainText[p] - 65) + (toupper(key[k]) - 65)) % 26;
                c = c + 65;
                printf("%c", (char)c);
            }
            else                            // sets lowercase '' and converts back to lowercase
            {
                c = ((plainText[p] - 97) + (toupper(key[k]) - 65)) % 26;
                c = c + 97;
                printf("%c", (char)c);
            }
        }
        else                // prints non-alphabetic characters
        {
            printf("%c", plainText[p]);
            k--;
        }
    }

    return 0;
}