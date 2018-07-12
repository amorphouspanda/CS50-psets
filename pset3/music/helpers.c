// Helper functions for music

#include <string.h>
#include <stdio.h>
#include <math.h>

#include "helpers.h"

// Converts a fraction formatted as X/Y to eighths
int duration(string fraction)
{
    const char *X = strtok(fraction, "/");
    const char *Y = strtok(NULL, "/");

    int X1 = atoi(X);
    int Y1 = atoi(Y);

    int n = 8 / Y1;

    return X1 * n;
}

// Calculates frequency (in Hz) of a note

int frequency(string note2)
{
    const char NOTES[] = {'C', 'D', 'E', 'F', 'G', 'A', 'B'};
    // const char ACC[] = {'b', '#'};
    // const int OCTAVE[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};

    char note[3];

    if (note2[1] != '#' && note2[1] != 'b')
    {
        note[0] = note2[0];
        note[1] = '\0';
        note[2] = note2[1];
    }
    else
    {
        note[0] = note2[0];
        note[1] = note2[1];
        note[2] = note2[2];
    }

    int semitones;

    // finds base semitones
    for (int a = 0, n = sizeof(NOTES) / sizeof(NOTES[0]); a < n; a++)
    {
        if (note[0] == NOTES[a])
        {
            if (a > 2)
            {
                semitones = 2 * (a - 5);
            }
            else
            {
                semitones = 2 * (a - 5) + 1;
                // printf("semitones: %d \n", semitones); // test print
            }
        }
    }

    // adjusts for accidentals
    if (note[1] == 'b')
    {
        semitones--;
    }
    else if (note[1] == '#')
    {
        semitones++;
    }

    int octave = note[2] - '0';
    // printf("octave: %d \n", octave);
    semitones += (octave - 4) * 12; // 12 = number of semitones in an octave
    // printf("total semitones: %d \n", semitones);

    int f = round(pow(2, (double)semitones / 12) * 440);
    // printf("frequency: %d \n", f);

    return f;
}

// Determines whether a string represents a rest
bool is_rest(string s)
{
    while (strcmp(s, "") == 0)
    {
        return true;
    }

    return false;
}
