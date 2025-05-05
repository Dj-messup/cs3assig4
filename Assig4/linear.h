#pragma once
#include <cstring>
#include "DynamicArray.h"

struct LinearEntry {
    char word[256];
    int count;
    bool occupied;

    LinearEntry() {
        word[0] = '\0';
        count = 0;
        occupied = false;
    }

    LinearEntry(const char* w) {
        strcpy(word, w);
        count = 1;
        occupied = true;
    }
};

class LinearTable {
private:
    DynamicArray<LinearEntry> table;
    int tableSize;
    int numElements;

    int hash(const char* word);
    int probeIndex(const char* word);
    void rehash();

public:
    LinearTable(int initialSize);
    ~LinearTable();
    void insert(const char* word);
    int getCount(const char* word);
};