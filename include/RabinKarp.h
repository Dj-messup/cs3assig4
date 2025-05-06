#ifndef RABIN_KARP_H
#define RABIN_KARP_H

#include "DynamicArray.h"
#include <string>

class RabinKarp
{
private:
    const int base;
    const int prime;
    DynamicArray<int> occurrences;

public:
    RabinKarp(int base = 256, int prime = 101);
    const DynamicArray<int> &search(const std::string &text, const std::string &pattern);
};

#endif