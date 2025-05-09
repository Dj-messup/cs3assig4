#ifndef PATTERN
#define PATTERN

#include "Linear.h"
#include "Chaining.h"
#include "DynamicArray.h"
#include "RabinKarp.h"
#include "Pair.h"
#include "Logger.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <iomanip>
#include <chrono>
#include <cstdlib>
#include <ctype.h>
#include <cstring>

class Pattern
{
private:
    HashTable* hashTable;
    LinearTable* linearTable;
    int workNum;
    int chainInsertCount;
    int linearInsertCount;
    int sentenceCount;
    int chainSize;
    int linearSize;

    struct FreqEntry
    {
        char word[256];
        int count;

        FreqEntry(const char* w = "")
        {
            strcpy(word, w);
            count = (w[0] == '\0') ? 0 : 1;
        }
    };

    DynamicArray<FreqEntry> wordCounts;
    DynamicArray<char*> workIXWords;

    void processWord(const char* input, char* output);
    void sortWordCounts(DynamicArray<FreqEntry>& wordCounts, bool descending);
    void outputFrequencyList(DynamicArray<FreqEntry>& wordCounts, const char* filename, const char* title);
    void strConv(char* word);

public:
    Pattern(int chainSize, int linearSize);
    ~Pattern();

    void readFile();
    void userSearch();
    void leastFrequent();
    void mostFrequent();
    int getCount(const char* word);
};

#endif