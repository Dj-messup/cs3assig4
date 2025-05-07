#include "Chaining.h"

HashTable::HashTable(int tableSize)
{
    this->tableSize = tableSize;
    table = new ChainNode*[tableSize];
    for (int i = 0; i < tableSize; ++i)
    {
        table[i] = nullptr;
    }
}

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
    delete[] table;
}

int HashTable::hash(const char* word)
{
    unsigned int hashValue = 0;
    for (int i = 0; word[i] != '\0'; ++i)
    {
        hashValue = word[i] + 31 * hashValue;
    }
    return hashValue % tableSize;
}

void HashTable::insert(const char* word)
{
    int index = hash(word);
    ChainNode* current = table[index];
    while (current != nullptr)
    {
        if (strcmp(current->word, word) == 0)
        {
            current->count++;
            return;
        }
        current = current->next;
    }
    table[index] = new ChainNode(word, table[index]);
}

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