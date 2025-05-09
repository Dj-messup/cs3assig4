#pragma once

#include "DynamicArray.h"
#include <cstring>

// Open-addressing hash table with linear probing
class LinearTable
{
private:
    // Each slot holds either a valid entry or is empty
    struct LinearEntry
    {
        char word[256];   // the key
        int count = 1;    // occurrences
        bool occupied = true;  // slot


        LinearEntry()
        {
            word[0] = '\0';
            count = 0;
            occupied = false;
        }

        LinearEntry(const char* w)
        {
            strcpy(word, w);
            count = 1;
            occupied = true;
        }
    };

    DynamicArray<LinearEntry> table;  
    int tableSize;      
    int numElements;    

    // Compute primary hash index
    int hash(const char* word);
    // Find where a word lives or should live
    int probeIndex(const char* word);
    // When table too full, grow and re-insert everything
    void rehash();

public:
    // Pass initial table size
    LinearTable(int tableSize);
    ~LinearTable();

    // Insert or update word
    void insert(const char* word);
    // Return how many times word was seen
    int getCount(const char* word);
};
