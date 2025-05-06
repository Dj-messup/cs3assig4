#include <iostream>
#include <fstream>
#include <cctype>
#include <cstring>
#include "matching.h"
#include "DynamicArray.h"

using namespace std;

void processWord(const char *input, char *output)
{
    int out_pos = 0;
    bool last_was_hyphen = false;

    for (int i = 0; input[i] != '\0' && out_pos < 256 - 1; i++)
    {
        char c = tolower(input[i]);

        if (c == '-')
        {
            if (!last_was_hyphen)
            {
                output[out_pos++] = c;
                last_was_hyphen = true;
            }
        }
        else
        {
            output[out_pos++] = c;
            last_was_hyphen = false;
        }
    }
    output[out_pos] = '\0';
}

void countWordFrequencies(const char *filename, DynamicArray<WordEntry> &wordCounts)
{
    ifstream inputFile(filename);
    if (!inputFile.is_open())
    {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    char word[256];
    char processed[256];

    while (inputFile >> word)
    {
        processWord(word, processed);

        if (strlen(processed) == 0)
            continue;

        bool found = false;
        for (size_t i = 0; i < wordCounts.getSize(); i++)
        {
            if (strcmp(wordCounts[i].word, processed) == 0)
            {
                wordCounts[i].count++;
                found = true;
                break;
            }
        }

        if (!found)
        {
            WordEntry wf(processed);
            wordCounts.push_back(wf);
        }
    }
    inputFile.close();
}

Pattern::Pattern() : hashTable(nullptr)
{
    countWordFrequencies("A_Scandal_In_Bohemia.txt", wordCounts);
}

Pattern::Pattern(int tableSize)
{
    hashTable = new HashTable(tableSize);
    countWordFrequencies("A_Scandal_In_Bohemia.txt", wordCounts);
}

Pattern::~Pattern()
{
    delete hashTable;
}

void sortWordCounts(DynamicArray<WordEntry> &wordCounts, bool descending)
{
    for (size_t i = 0; i < wordCounts.getSize() - 1; i++)
    {
        for (size_t j = 0; j < wordCounts.getSize() - i - 1; j++)
        {
            bool shouldSwap = descending ? (wordCounts[j].count < wordCounts[j + 1].count) : (wordCounts[j].count > wordCounts[j + 1].count);

            if (shouldSwap)
            {
                WordEntry temp = wordCounts[j];
                wordCounts[j] = wordCounts[j + 1];
                wordCounts[j + 1] = temp;
            }
        }
    }
}

void outputFrequencyList(DynamicArray<WordEntry> &wordCounts, const char *filename, const char *title)
{
    ofstream outputFile(filename, ios::app);
    if (!outputFile.is_open())
    {
        cerr << "Error opening output file: " << filename << endl;
        return;
    }

    outputFile << "\n=== " << title << " ===\n";
    outputFile << "Word\t\tFrequency\n";
    outputFile << "-----------------------\n";

    size_t limit = wordCounts.getSize() > 80 ? 80 : wordCounts.getSize();
    for (size_t i = 0; i < limit; i++)
    {
        outputFile << wordCounts[i].word << "\t\t"
                   << wordCounts[i].count << "\n";
    }

    outputFile.close();
}

void Pattern::leastFrequent()
{
    DynamicArray<WordEntry> leastFrequent = wordCounts;
    sortWordCounts(leastFrequent, false);
    outputFrequencyList(leastFrequent, "frequencies.txt", "80 Least Frequent Words");
}

void Pattern::mostFrequent()
{
    DynamicArray<WordEntry> mostFrequent = wordCounts;
    sortWordCounts(mostFrequent, true);
    outputFrequencyList(mostFrequent, "frequencies.txt", "80 Most Frequent Words");
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

void Pattern::strConv(char *word)
{
    int len = strlen(word);
    int j = 0;
    for (int i = 0; i < len; ++i)
    {
        if (word[i] == '-' && word[i + 1] == '-')
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
    table = new WordEntry *[tableSize];
    for (int i = 0; i < tableSize; ++i)
    {
        table[i] = nullptr;
    }
}

HashTable::~HashTable()
{
    for (int i = 0; i < tableSize; ++i)
    {
        WordEntry *current = table[i];
        while (current != nullptr)
        {
            WordEntry *toDelete = current;
            current = current->next;
            delete toDelete;
        }
    }
    delete[] table;
}

int HashTable::hash(const char *word)
{
    unsigned int hashValue = 0;
    for (int i = 0; word[i] != '\0'; ++i)
    {
        hashValue = (hashValue * 31) + word[i];
    }
    return hashValue % tableSize;
}

void HashTable::insert(const char *word)
{
    int index = hash(word);
    WordEntry *current = table[index];
    while (current != nullptr)
    {
        if (strcmp(current->word, word) == 0)
        {
            current->count++;
            return;
        }
        current = current->next;
    }
    WordEntry *newEntry = new WordEntry(word);
    newEntry->next = table[index];
    table[index] = newEntry;
}