#pragma once
#include <cstring>

struct WordEntry
{
    char word[256];
    int count;
    WordEntry *next;
    WordEntry(const char *w)
    {
        strcpy(word, w);
        count = 1;
        next = nullptr;
    }
};

class HashTable
{
private:
    WordEntry **table;
    int tableSize;

public:
    HashTable(int size);
    ~HashTable();
    int hash(const char *word);
    void insert(const char *word);
};

class Pattern
{
private:
    HashTable *hashTable;
    DynamicArray<WordEntry> wordCounts;

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