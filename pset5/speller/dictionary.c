// Implements a dictionary's functionality

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>

#include "dictionary.h"

node* makeNode();
void insert(node* root, char* key);
bool search(node* root, const char* key);
bool becauseNoParameterIsImpossibleForRecursionAndAFuckingBitch(node* root);

int* wordsLoaded = NULL;
node* first = NULL;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    return search(first, word);
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE *loaded = fopen(dictionary, "r");

    if (loaded == NULL)
    {
        return false;
    }

    wordsLoaded = malloc(sizeof(int));
    *wordsLoaded = 0;

    char* buffer = malloc(sizeof(char) * LENGTH);

    for (int a = 0; a < (sizeof(buffer) / sizeof(buffer[0])); a++)
    {
        buffer[a] = '\0';
    }

    first = makeNode();

    while (fscanf(loaded, "%s", buffer) == 1)
    {
        for (int a = 0; buffer[a] != '\0'; a++)
        {
            insert(first, buffer);
        }

        (*wordsLoaded)++;
    }

    fclose(loaded);
    free(buffer);

    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    if (!wordsLoaded)
    {
        return 0;
    }
    else
    {
        return *wordsLoaded;
    }
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    free(wordsLoaded);
    return becauseNoParameterIsImpossibleForRecursionAndAFuckingBitch(first);
}

bool becauseNoParameterIsImpossibleForRecursionAndAFuckingBitch(node* root)
{
    node* cursor = root;

    for (int a = 0; a < ALPHABET; a++)
    {
        if (cursor->nextLetter[a] != NULL)
        {
            becauseNoParameterIsImpossibleForRecursionAndAFuckingBitch(cursor->nextLetter[a]);
        }
    }

    free(cursor);

    return true;
}

node* makeNode()
{
    node* newNode = malloc(sizeof(node));
    newNode->isWord = false;

    for (int a = 0; a < ALPHABET; a++)
    {
        newNode->nextLetter[a] = NULL;
    }

    return newNode;
}

void insert(node* root, char* key)
{
    int bufferIndex;
    node* child = root;

    for (int a = 0; key[a] != '\0'; a++)
    {
        if (key[a] == '\'')
        {
            bufferIndex = 26;
        }
        else
        {
            bufferIndex = key[a] - 'a';
        }
        // printf("%d\n", bufferIndex);

        if (child->nextLetter[bufferIndex] == NULL)
        {
            child->nextLetter[bufferIndex] = makeNode();
        }

        child = child->nextLetter[bufferIndex];
    }

    child->isWord = true;
}

// should probably just be check but too lazy
bool search(node* root, const char* key)
{
    int bufferIndex;
    node* checker = root;

    for (int a = 0; key[a] != '\0'; a++)
    {
        if (key[a] == '\'')
        {
            bufferIndex = 26;
        }
        else
        {
            bufferIndex = tolower(key[a]) - 'a';
        }

        if (checker->nextLetter[bufferIndex] != NULL)
        {
            checker = checker->nextLetter[bufferIndex];
        }
        else
        {
            return false;
        }
    }

    return checker->isWord;
}
