#include "Linear.h"

// Store tableSize and start with numElements = 0
LinearTable::LinearTable(int tableSize)
{
    this->tableSize = tableSize;
    numElements = 0;
    for (int i = 0; i < tableSize; ++i)
    {
        table.push_back(LinearEntry());  // fill with empty entries
    }
}

LinearTable::~LinearTable() {}

// Same rolling‐hash as chaining
int LinearTable::hash(const char* word)
{
    unsigned int hashValue = 0;
    for (int i = 0; word[i] != '\0'; ++i)
    {
        hashValue = word[i] + 31 * hashValue;
    }
    return hashValue % tableSize;
}

// Linear probe: scan forward until empty or match
int LinearTable::probeIndex(const char* word)
{
    int index = hash(word);
    int startIndex = index;

    // keep going until we find slot not occupied or matching word
    while (table[index].occupied && strcmp(table[index].word, word) != 0)
    {
        index = (index + 1) % tableSize;  // wrap around
        if (index == startIndex)
            return -1;  // full cycle → nowhere to insert
    }
    return index;
}

// Insert new or bump count if exists; rehash at 50% load
void LinearTable::insert(const char* word)
{
    // ** load factor check (we kept it at 0.5 for simplicity) **
    if ((double)numElements / tableSize > 0.5)
    {
        rehash();  // double size and reinsert all keys
    }

    int index = probeIndex(word);
    // if slot is used and same word → update
    if (table[index].occupied && strcmp(table[index].word, word) == 0)
    {
        table[index].count++;
    }
    else
    {
        // empty slot: place new entry
        table[index] = LinearEntry(word);
        numElements++;
    }
}

// Double tableSize, copy old table out and re‐insert
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

    numElements = 0;  // will recount as we reinsert

    // reinsert each old entry
    for (int i = 0; i < oldSize; ++i)
    {
        if (oldTable[i].occupied)
        {
            insert(oldTable[i].word);
            // after insertion, restore the original count
            table[probeIndex(oldTable[i].word)].count = oldTable[i].count;
        }
    }
}

// Get count or 0 if not found
int LinearTable::getCount(const char* word)
{
    int index = probeIndex(word);
    if (index >= 0 && table[index].occupied && strcmp(table[index].word, word) == 0)
    {
        return table[index].count;
    }
    return 0;
}
