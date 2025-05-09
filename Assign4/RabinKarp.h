#pragma once

#include "DynamicArray.h"
#include <string>

//Rabin-Karp string search with rolling hash (Horner’s rule)
class RabinKarp
{
private:
    const int base;         // e.g. 256
    const int prime;        // e.g. 101
    DynamicArray<int> occurrences;  // store match indices

public:
    RabinKarp(int base = 256, int prime = 101);
    const DynamicArray<int> &search(const std::string &text, const std::string &pattern);
};

