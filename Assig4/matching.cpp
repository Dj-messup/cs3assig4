#include <iostream>
#include <fstream>
#include <cctype>
#include <cstring>
#include "matching.h"

using namespace std;

Pattern::Pattern() {
    hashTable = nullptr;
    linearTable = nullptr;
    workNum = 0;
}

Pattern::Pattern(int tableSize) {
    hashTable = new HashTable(tableSize);
    linearTable = new LinearTable(tableSize);
    workNum = 0;
}

Pattern::~Pattern() {
    delete hashTable;
    delete linearTable;
}

void Pattern::readFile() {
    ifstream inFile("A Scandal In Bohemia.txt");
    if (!inFile) {
        cerr << "Error: File could not be opened." << endl;
        exit(1);
    }

    char word[256];
    while (inFile >> word) {
        strConv(word);

        if (strlen(word) == 1 && word[0] >= 'i' && word[0] <= 'x') {
            workNum++;
            continue;
        }

        if (strlen(word) == 0) continue;

        if (workNum >= 1 && workNum <= 6) {
            hashTable->insert(word);
        } else if (workNum >= 7 && workNum <= 12) {
            linearTable->insert(word);
        }
    }

    inFile.close();
}

void Pattern::strConv(char* word) {
    int len = strlen(word);
    int j = 0;
    for (int i = 0; i < len; ++i) {
        if (word[i] == '-' && word[i + 1] == '-') {
            ++i;
            continue;
        }
        if (isalpha(word[i]) || word[i] == '-') {
            word[j++] = tolower(word[i]);
        }
    }
    word[j] = '\0';
}

HashTable::HashTable(int size) {
    tableSize = size;
    table = new WordEntry*[tableSize];
    for (int i = 0; i < tableSize; ++i) {
        table[i] = nullptr;
    }
}

HashTable::~HashTable() {
    for (int i = 0; i < tableSize; ++i) {
        WordEntry* current = table[i];
        while (current != nullptr) {
            WordEntry* toDelete = current;
            current = current->next;
            delete toDelete;
        }
    }
    delete[] table;
}

int HashTable::hash(const char* word) {
    unsigned int hashValue = 0;
    for (int i = 0; word[i] != '\0'; ++i) {
        hashValue = (hashValue * 31) + word[i];
    }
    return hashValue % tableSize;
}

void HashTable::insert(const char* word) {
    int index = hash(word);
    WordEntry* current = table[index];
    while (current != nullptr) {
        if (strcmp(current->word, word) == 0) {
            current->count++;
            return;
        }
        current = current->next;
    }
    WordEntry* newEntry = new WordEntry(word);
    newEntry->next = table[index];
    table[index] = newEntry;
}

int HashTable::getCount(const char* word) {
    int index = hash(word);
    WordEntry* current = table[index];
    while (current != nullptr) {
        if (strcmp(current->word, word) == 0) {
            return current->count;
        }
        current = current->next;
    }
    return 0;
}