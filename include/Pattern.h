#ifndef PATTERN
#define PATTERN

#include "Linear.h"
#include "Chaining.h"
#include "DynamicArray.h"
#include <iostream>
#include <fstream>

class Pattern
{
private:
    HashTable *hashTable;
    LinearTable *linearTable;
    int workNum;

    struct FreqEntry
    {
        char word[256];
        int count;

        FreqEntry(const char *w = "")
        {
            strcpy(word, w);
            count = (w[0] == '\0') ? 0 : 1;
        }
    };

    DynamicArray<FreqEntry> wordCounts;

    void processWord(const char *input, char *output);
    void countWordFrequencies(const char *filename, DynamicArray<FreqEntry> &wordCounts);
    void sortWordCounts(DynamicArray<FreqEntry> &wordCounts, bool descending);
    void outputFrequencyList(DynamicArray<FreqEntry> &wordCounts, const char *filename, const char *title);

public:
    Pattern(int tableSize);
    ~Pattern();
    void leastFrequent();
    void mostFrequent();
    void readFile();
    void userSearch();
    void strConv(char *);
    int getCount(const char *word);
};

#endif
