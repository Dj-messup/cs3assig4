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
    DynamicArray<WordEntry> wordCounts;
    void processWord(const char *input, char *output);
    void countWordFrequencies(const char *filename, DynamicArray<WordEntry> &wordCounts);
    void sortWordCounts(DynamicArray<WordEntry> &wordCounts, bool descending);
    void outputFrequencyList(DynamicArray<WordEntry> &wordCounts, const char *filename, const char *title);

public:
    Pattern();
    Pattern(int tableSize);
    ~Pattern();
    void leastFrequent();
    void mostFrequent();
    void readFile();
    void userSearch();
    void strConv(char *);
};

#endif