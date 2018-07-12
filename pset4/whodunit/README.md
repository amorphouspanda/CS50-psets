# Questions

## What's `stdint.h`?

a header that allows for the declaration of integers with specified widths

## What's the point of using `uint8_t`, `uint32_t`, `int32_t`, and `uint16_t` in a program?

if you're certain you won't use negative integers, those can store larger numbers
also makes it clear what you intend to use the integer for

## How many bytes is a `BYTE`, a `DWORD`, a `LONG`, and a `WORD`, respectively?

1, 4, 4, 2

## What (in ASCII, decimal, or hexadecimal) must the first two bytes of any BMP file be? Leading bytes used to identify file formats (with high probability) are generally called "magic numbers."

BM

## What's the difference between `bfSize` and `biSize`?

bfsize: size of entire bmp file
bisize: size of BITMAPINFOHEADER header file

## What does it mean if `biHeight` is negative?

the bmp is a top-down dib with the origin at the upper left corner

## What field in `BITMAPINFOHEADER` specifies the BMP's color depth (i.e., bits per pixel)?

biBitCount

## Why might `fopen` return `NULL` in lines 24 and 32 of `copy.c`?

if files infile or outfile don't exist/can't be found

## Why is the third argument to `fread` always `1` in our code?

1 RGB value (1 RGBTRIPLE structure) is read at a time

## What value does line 63 of `copy.c` assign to `padding` if `bi.biWidth` is `3`?

3

## What does `fseek` do?

fseek sets the position of the "reader" to the specified place in the file

## What is `SEEK_CUR`?

sets the position of the "reader" to the specified number of bytes after the current position
