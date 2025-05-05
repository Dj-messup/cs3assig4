#pragma once
#include "chaining.h"
#include "linear.h"

class Pattern {
private:
    HashTable* hashTable;
    LinearTable* linearTable;
    int workNum;

public:
    Pattern();
    Pattern(int tableSize);
    ~Pattern();
    void leastFrequent();
    void mostFrequent();
    void readFile();
    void userSearch();
    void strConv(char*);
};