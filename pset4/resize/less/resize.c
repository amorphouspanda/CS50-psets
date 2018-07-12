// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: copy infile outfile\n");
        return 1;
    }

    // remember filenames
    float scale = atof(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];

    if (scale <= 0 || scale > 100) {
        fprintf(stderr, "Input scale between (0, 100]");
        return 1;
    }

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    BITMAPINFOHEADER new_bi = bi;
    BITMAPFILEHEADER new_bf = bf;

    // resizing headers
    new_bi.biWidth = bi.biWidth * scale;
    new_bi.biHeight = bi.biHeight * scale;

    // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int new_padding = (4 - (new_bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // more header resizing
    new_bi.biSizeImage = ((sizeof(RGBTRIPLE) * new_bi.biWidth) + new_padding) * abs(new_bi.biHeight);
    new_bf.bfSize = new_bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&new_bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&new_bi, sizeof(BITMAPINFOHEADER), 1, outptr);

/*
// IDK HOW TO DO RESIZE MORE FUCK THIS
    float range = 1;
    if (scale < 1)
    {
        range = ceil(bi.biWidth * scale);
    }

    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i += (range - 1))
    {
        for (int b = 0; b < scale; b++) {
            // iterate over pixels in scanline
            for (int j = 0; j < bi.biWidth; j += range)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // write RGB triple to outfile
                for (int a = 0; a < scale; a++)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }

            }

            // then add it back (to demonstrate how)
            for (int k = 0; k < new_padding; k++)
            {
                fputc(0x00, outptr);
            }

            if (b < (scale - 1))
            {
                fseek(inptr, -1 * (bi.biWidth * sizeof(RGBTRIPLE)), SEEK_CUR);
            }
        }

        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);
    }
*/

// for scale is int and > 1
    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        for (int b = 0; b < scale; b++) {
            // iterate over pixels in scanline
            for (int j = 0; j < bi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // write RGB triple to outfile
                for (int a = 0; a < scale; a++)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }

            // then add it back (to demonstrate how)
            for (int k = 0; k < new_padding; k++)
            {
                fputc(0x00, outptr);
            }

            if (b < (scale - 1))
            {
                fseek(inptr, -1 * (bi.biWidth * sizeof(RGBTRIPLE)), SEEK_CUR);
            }
        }

        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
