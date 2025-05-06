#include "Linear.h"

LinearTable::LinearTable(int initialSize)
{
    tableSize = initialSize;
    numElements = 0;
    for (int i = 0; i < tableSize; ++i)
    {
        table.push_back(LinearEntry());
    }
}

LinearTable::~LinearTable() {}

int LinearTable::hash(const char *word)
{
    unsigned int hashValue = 0;
    for (int i = 0; word[i] != '\0'; ++i)
    {
        hashValue = hashValue + word[i];
    }
    return hashValue % tableSize;
}

int LinearTable::probeIndex(const char *word)
{
    int index = hash(word);
    int startIndex = index;

    while (table[index].occupied && strcmp(table[index].word, word) != 0)
    {
        index = (index + 1) % tableSize;
        if (index == startIndex)
        {
            return -1;
        }
    }
    return index;
}

void LinearTable::insert(const char *word)
{
    if ((double)numElements / tableSize > 0.5)
    {
        rehash();
    }

    int index = probeIndex(word);
    if (table[index].occupied && strcmp(table[index].word, word) == 0)
    {
        table[index].count++;
    }
    else
    {
        table[index] = LinearEntry(word);
        numElements++;
    }
}

void LinearTable::rehash()
{
    int oldSize = tableSize;
    tableSize *= 2;
    DynamicArray<LinearEntry> oldTable = table;

    table = DynamicArray<LinearEntry>();
    for (int i = 0; i < tableSize; ++i)
    {
        table.push_back(LinearEntry());
    }

    numElements = 0;

    for (int i = 0; i < oldSize; ++i)
    {
        if (oldTable[i].occupied)
        {
            insert(oldTable[i].word);
            table[probeIndex(oldTable[i].word)].count = oldTable[i].count;
        }
    }
}

int LinearTable::getCount(const char *word)
{
    int index = probeIndex(word);
    if (table[index].occupied && strcmp(table[index].word, word) == 0)
    {
        return table[index].count;
    }
    return 0;
}