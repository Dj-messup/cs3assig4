#ifndef CHAINING
#define CHAINING

#include "WordEntry.h"

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
    int getCount(const char *word);
};

#endif