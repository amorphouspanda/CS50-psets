#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Enter 1 command-line argument");
        return 1;
    }

    char *infile = argv[1];

    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // read file in blocks of 512B until you find jpeg markings

    int start = 0;
    int name = 0;
    unsigned char buffer[512];
    FILE *outptr = NULL;

    while (fread(buffer, 512, 1, inptr) == 1)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)  // start/stop?
        {
            if (start == 0)
            {
                start = 1;
            }
            else
            {
                fclose(outptr);
            }

            char filename[8];
            sprintf(filename, "%03d.jpg", name);
            outptr = fopen(filename, "a");
            name++;
        }

        if (start == 1)
        {
            fwrite(&buffer, 512, 1, outptr);
        }
    }

    fclose(outptr);
    fclose(inptr);

    return 0;
}

