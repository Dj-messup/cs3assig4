#include <iostream>
#include <fstream>
#include <cctype>
#include <cstring>
#include "matching.h"

using namespace std;

Pattern::Pattern()
{
    hashTable=nullptr;
}

Pattern::Pattern(int tableSize)
{
    hashTable = new HashTable(tableSize);
}

Pattern::~Pattern()
{
    delete hashTable;
}

void Pattern::readFile()
{
    ifstream inFile("A Scandal In Bohemia.txt");
    if (!inFile)
    {
        cerr << "Error: File could not be opened." << endl;
        exit(1);
    }
    char word[256];
    while (inFile >> word)
    {
        strConv(word);
        hashTable->insert(word);
    }
    inFile.close();
}

void Pattern::strConv(char* word)
{
    int len = strlen(word);
    int j = 0;
    for (int i = 0; i < len; ++i)
    {
        if (word[i] == '-' && word[i+1] == '-')
        {
            ++i;
            continue;
        }
        if (isalpha(word[i]) || word[i] == '-')
        {
            word[j++] = tolower(word[i]);
        }
    }
    word[j] = '\0';
}

HashTable::HashTable(int size)
{
    tableSize = size;
    table = new WordEntry*[tableSize];
    for (int i = 0; i < tableSize; ++i)
    {
        table[i] = nullptr;
    }
}

HashTable::~HashTable()
{
    for (int i = 0; i < tableSize; ++i)
    {
        WordEntry* current = table[i];
        while (current != nullptr)
        {
            WordEntry* toDelete = current;
            current = current->next;
            delete toDelete;
        }
    }
    delete[] table;
}

int HashTable::hash(const char* word)
{
    unsigned int hashValue = 0;
    for (int i = 0; word[i] != '\0'; ++i)
    {
        hashValue = (hashValue * 31) + word[i];
    }
    return hashValue % tableSize;
}

void HashTable::insert(const char* word)
{
    int index = hash(word);
    WordEntry* current = table[index];
    while (current != nullptr)
    {
        if (strcmp(current->word, word) == 0)
        {
            current->count++;
            return;
        }
        current = current->next;
    }
    WordEntry* newEntry = new WordEntry(word);
    newEntry->next = table[index];
    table[index] = newEntry;
}