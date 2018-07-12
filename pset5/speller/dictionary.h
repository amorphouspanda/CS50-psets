// Declares a dictionary's functionality

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>

// Maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 45
#define ALPHABET 27

// Prototypes

bool check(const char *word);
bool load(const char *dictionary);
unsigned int size(void);
bool unload(void);

typedef struct node
{
    bool isWord;
    struct node* nextLetter[ALPHABET];
}
node;

#endif // DICTIONARY_H
