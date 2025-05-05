#pragma once
#include <cstring>
#include "DynamicArray.h"

struct WordEntry {
    char word[256];
    int count;
    WordEntry* next;

    WordEntry(const char* w) {
        strcpy(word, w);
        count = 1;
        next = nullptr;
    }
};

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

class HashTable {
    private:
        WordEntry** table;
        int tableSize;

    public:
        HashTable(int size);
        ~HashTable();
        int hash(const char* word);
        void insert(const char* word);
        int getCount(const char* word);
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

class Pattern {
    private:
        HashTable* hashTable;
        LinearTable* linearTable;
        int workNum;

    public:
        Pattern();
        Pattern(int tableSize);
        ~Pattern();
        void leastFrequent();
        void mostFrequent();
        void readFile();
        void userSearch();
        void strConv(char* word);
};