#pragma once

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

// This class coordinates everything:
//  - single-pass reading & dispatching to hash or linear tables
//  - sentence counting
//  - storing words of Work IX for userSearch()
//  - frequency lists
//  - timing & logging
class Pattern
{
private:
    HashTable* hashTable;        // for works I–VI
    LinearTable* linearTable;    // for works VII–XII
    int workNum;                 // which of the 12 stories we’re in
    int chainInsertCount;        // how many items went into chaining
    int linearInsertCount;       // how many items into linear probing
    int sentenceCount;           // running total of sentences seen
    int chainSize;               // initial chaining table size
    int linearSize;              // initial linear table size

    // Temporary struct for frequency counting
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

    DynamicArray<FreqEntry> wordCounts;    // all distinct words seen
    DynamicArray<char*> workIXWords;       // to store words of Work IX

    // PRIVATE HELPERS:

    // 1) clean up a raw input token: remove double-hyphens, force lowercase,
    //    preserve single hyphens, drop non-alpha
    void processWord(const char* input, char* output);

    // 2) full filter to drop “--” and keep only letters+hyphens
    void strConv(char* word);

    // 3) basic bubble-sort on the wordCounts array
    void sortWordCounts(DynamicArray<FreqEntry>& wordCounts, bool descending);

    // 4) write the top/least list into frequencies.txt
    void outputFrequencyList(DynamicArray<FreqEntry>& wordCounts, const char* filename, const char* title);

public:
    // constructor: set up hash & linear tables, counters zeroed
    Pattern(int chainSize, int linearSize);
    ~Pattern();

    // MAJOR OPERATIONS:
    // readFile(): open the big text, single-pass loop,
    //             count sentences, update tables, record work IX,
    //             log chain/linear stats, run userSearch(), & freq lists
    void readFile();

    // ask user for up to 8 patterns, run simple scan over workIXWords
    // (in-class we’d use Rabin-Karp, but here we just match strings)
    void userSearch();

    // dump 80 least / most frequent words
    void leastFrequent();
    void mostFrequent();

    // helper to get combined count from both tables
    int getCount(const char* word);
};
