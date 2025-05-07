#ifndef LINEAR
#define LINEAR

#include "DynamicArray.h"
#include <cstring>

class LinearTable
{
private:
    struct LinearEntry
    {
        char word[256];
        int count = 1;
        bool occupied = true;

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

    int hash(const char* word);
    int probeIndex(const char* word);
    void rehash();

public:
    LinearTable(int tableSize);
    ~LinearTable();
    void insert(const char* word);
    int getCount(const char* word);
};

#endif