#include "Chaining.h"

// Constructor: allocate buckets and set them to empty
HashTable::HashTable(int tableSize)
{
    this->tableSize = tableSize;
    table = new ChainNode*[tableSize];
    for (int i = 0; i < tableSize; ++i)
    {
        table[i] = nullptr;  // no chains yet
    }
}

// Destructor: walk each bucket’s list and delete every node
HashTable::~HashTable()
{
    for (int i = 0; i < tableSize; ++i)
    {
        ChainNode* current = table[i];
        while (current != nullptr)
        {
            ChainNode* next = current->next;
            delete current;
            current = next;
        }
    }
    delete[] table;  // free bucket array
}

// A classic polynomial rolling hash: accumulate bytes, multiply by 31
int HashTable::hash(const char* word)
{
    unsigned int hashValue = 0;
    for (int i = 0; word[i] != '\0'; ++i)
    {
        hashValue = word[i] + 31 * hashValue;
    }
    return hashValue % tableSize;
}

// Insert word: if found, increment count; otherwise push new node at front
void HashTable::insert(const char* word)
{
    int index = hash(word);
    ChainNode* current = table[index];
    // scan list to see if word already there
    while (current != nullptr)
    {
        if (strcmp(current->word, word) == 0)
        {
            current->count++;  // seen before → bump counter
            return;
        }
        current = current->next;
    }
    // not found → create new node at head
    table[index] = new ChainNode(word, table[index]);
}

// Return count if found, else 0
int HashTable::getCount(const char* word)
{
    int index = hash(word);
    ChainNode* current = table[index];
    while (current != nullptr)
    {
        if (strcmp(current->word, word) == 0)
        {
            return current->count;
        }
        current = current->next;
    }
    return 0;
}
