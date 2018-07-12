#include <stdio.h>
#include <string.h>
#include <math.h>

int frequency(char note[]);

int main() {
    char note[4] = {'C', '\0', '4'};

    int f = frequency(note);

    printf("%d \n", f);
}

int frequency(char note[]) {
    const char NOTES[] = {'C', 'D', 'E', 'F', 'G', 'A', 'B'};
    // const char ACC[] = {'b', '#'};
    // const int OCTAVE[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};

    int semitones;

    // finds base semitones
    for (int a = 0, n = sizeof(NOTES) / sizeof(NOTES[0]); a < n; a++) {
        if (note[0] == NOTES[a]) {
            if (a > 2) {
                semitones = 2 * (a - 5);
            } else {
                semitones = 2 * (a - 5) + 1;
                // printf("semitones: %d \n", semitones); // test print
            }
        }
    }

    // adjusts for accidentals
    if (note[1] == 'b') {
        semitones--;
    } else if (note[1] == '#') {
        semitones++;
    }

    int octave = note[2] - '0';
    // printf("octave: %d \n", octave);
    semitones += (octave - 4) * 12; // 12 = number of semitones in an octave
    // printf("total semitones: %d \n", semitones);

    int f = (pow(2, (double)semitones / 12) * 440);
    // printf("frequency: %d \n", f);

    return f;
}