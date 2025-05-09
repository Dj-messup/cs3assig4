#pragma once

#include "Chaining.h"
#include "Linear.h"
#include "DynamicArray.h"
#include "Pair.h"
#include "Logger.h"

#include <iostream>
#include <fstream>
#include <chrono>
#include <cctype>
#include <cstring>
#include <cstdlib>

class Pattern
{
private:
    HashTable*          hashTable;       // works I–VI
    LinearTable*        linearTable;     // works VII–XII
    int                 workNum;         // current story number
    int                 chainInsertCount;
    int                 linearInsertCount;
    int                 sentenceCount;
    int                 chainSize;
    int                 linearSize;

    struct FreqEntry
    {
        char word[256];
        int  count;
        FreqEntry(const char* w = "")
        {
            std::strcpy(word, w);
            count = (w[0] ? 1 : 0);
        }
    };

    DynamicArray<FreqEntry> wordCounts;   // global freqs
    DynamicArray<char*>     workIXWords;   // tokens from story IX

    void processWord(const char* input, char* output);
    void strConv(char* word);
    void sortWordCounts(DynamicArray<FreqEntry>& arr, bool desc);
    void outputFrequencyList(DynamicArray<FreqEntry>& arr,
                             const char* filename,
                             const char* title);

public:
    Pattern(int chainSize, int linearSize);
    ~Pattern();

    // Read the given file, route tokens into hash/linear tables,
    // count sentences, build freq list, and *only* store IX tokens.
    void readFile(const char* filename);

    void userSearch();
    void mostFrequent();
    void leastFrequent();
    int  getCount(const char* word);
};
