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
#include <cstring>

class Pattern
{
private:
    HashTable *hashTable;
    LinearTable *linearTable;
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
    std::string extractStoryIX(const std::string &filename);
    DynamicArray<std::string> splitString(const std::string &input, const std::string &delimiter);
    DynamicArray<Pair<std::string, size_t>> getWordsWithPositions(const std::string &text);

public:
    Pattern(int chainSize, int linearSize);
    ~Pattern();
    void leastFrequent();
    void mostFrequent();
    void readFile();
    void userSearch();
    void strConv(char *);
    int getCount(const char *word);
};

#endif
