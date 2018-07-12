#include <stdio.h>
#include <stdlib.h>

int main() {
    int name = 0;

    char outfile;

    sprintf(&outfile, "%03d", name);

    FILE *outptr = fopen(&outfile, "w");
    fclose(outptr);
}