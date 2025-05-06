#ifndef WORD_ENTRY
#define WORD_ENTRY

#include <cstring>

struct WordEntry
{
    char word[256];
    int count;
    WordEntry *next;
    WordEntry() : word{}, count(0), next(nullptr) {}
    WordEntry(const char *w)
    {
        strcpy(word, w);
        count = 1;
        next = nullptr;
    }
};

#endif