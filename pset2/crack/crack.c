#define _XOPEN_SOURCE
#include <unistd.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLENGTH 5
#define ALPHALENGTH 52

void reset(int start);

char* cracked;

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("Wrong, try again\n");
        return 1;
    }

    const char* salt = "50";

    const char* inputHash = argv[1];

    char *alpha = "ROFLQWETYUIPASDGHJKZXCVBNMqwertyuiopasdfghjklzxcvbnm";

    //const char* salt = malloc(sizeof(char) * 2);
    //salt[0] = &inputHash[0];
    //salt[1] = &inputHash[1];

    cracked = malloc(sizeof(char) * (MAXLENGTH + 1));
    cracked[5] = '\0';
    reset(0);

    for (int a = 0; a < ALPHALENGTH; a++)
    {
        cracked[0] = alpha[a];
        if (strcmp(crypt(cracked, salt), inputHash) == 0)
        {
            goto DONE;
        }

        for (int b = 0; b < ALPHALENGTH; b++)
        {
            cracked[1] = alpha[b];
            if (strcmp(crypt(cracked, salt), inputHash) == 0)
            {
                goto DONE;
            }

            for (int c = 0; c < ALPHALENGTH; c++)
            {
                cracked[2] = alpha[c];
                if (strcmp(crypt(cracked, salt), inputHash) == 0)
                {
                    goto DONE;
                }

                for (int d = 0; d < ALPHALENGTH; d++)
                {
                    cracked[3] = alpha[d];
                    // printf("%s\n", cracked);

                    if (strcmp(crypt(cracked, salt), inputHash) == 0)
                    {
                        goto DONE;
                    }

                    for (int e = 0; e < ALPHALENGTH; e++)
                    {
                        cracked[4] = alpha[e];
                        // printf("%s\n", cracked);
                        if (strcmp(crypt(cracked, salt), inputHash) == 0)
                        {
                            goto DONE;
                        }
                    }

                    reset(4);
                }

                reset(3);
            }

            reset(2);
        }

        reset(1);
    }

    DONE: printf("%s\n", cracked);
    free(cracked);
}

void reset(int start)
{
    for (int a = start; a < MAXLENGTH; a++)
    {
        cracked[a] = '\0';
    }
}

