//#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Prompt user for height
    int height = get_int("height: ");

    // Check height
    while (height > 23 || height < 0)
    {
        height = get_int("height: ");
    }

    // Make pyramid
    for (int a = 1; a <= height; a++)
    {
        // Writing spaces
        for (int c = 0; c < height - a; c++)
        {
            printf(" ");
        }

        // Writing hashes
        for (int d = 0; d < a; d++)
        {
            printf("#");
        }

        printf(" ");
        printf(" ");

        for (int d = 0; d < a; d++)
        {
            printf("#");
        }

        printf("\n");
    }
}