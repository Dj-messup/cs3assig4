#include <cstring>
#include "chaining.h"

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
        hashValue = hashValue + word[i];
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