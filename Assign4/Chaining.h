#ifndef CHAINING
#define CHAINING

#include <cstring>

class HashTable
{
private:
    struct ChainNode
    {
        char word[256];
        int count = 1;
        ChainNode* next = nullptr;

        ChainNode(const char* w, ChainNode* n)
        {
            strcpy(word, w);
            next = n;
        }
    };

    ChainNode** table;
    int tableSize;

public:
    HashTable(int tableSize);
    ~HashTable();
    int hash(const char* word);
    void insert(const char* word);
    int getCount(const char* word);
};

#endif