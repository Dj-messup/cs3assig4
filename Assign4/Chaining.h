#pragma once

#include <cstring>

// Simple hash table using separate chaining (linked lists)
// Each bucket is a linked list of ChainNode
class HashTable
{
private:
    // Node in the chain
    struct ChainNode
    {
        char word[256];        // the key, fixed-size C-string
        int count = 1;         // how many times we’ve seen this word
        ChainNode* next = nullptr;  // pointer to next node

        // Constructor to copy incoming word and link it in
        ChainNode(const char* w, ChainNode* n)
        {
            strcpy(word, w);   // copy word into our buffer
            next = n;          // link to existing list
        }
    };

    ChainNode** table;  // array of pointers, each to head of a chain
    int tableSize;      // number of buckets

public:
    // Create table with given number of buckets, initialize each to nullptr
    HashTable(int tableSize);

    // Clean up all chains and delete all nodes
    ~HashTable();

    // Hash function: Horner’s rule mod tableSize
    int hash(const char* word);

    // Insert or update a word in the chain
    void insert(const char* word);

    // Return how many times a word appears (0 if not found)
    int getCount(const char* word);
};
