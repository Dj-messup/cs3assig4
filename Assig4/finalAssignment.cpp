#include <iostream>
#include <fstream>
#include <cctype>
#include <cstring>
#include "finalAssignment.h"

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
    std::ifstream inFile("A Scandal In Bohemia.txt");
    if (!inFile) {
        std::cerr << "Error: File could not be opened." << std::endl;
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
        if (word[i] == '-' && word[i+1] == '-') {
            ++i;
            continue;
        }
        if (isalpha(word[i]) || word[i] == '-') {
            word[j++] = tolower(word[i]);
        }
    }
    word[j] = '\0';
}