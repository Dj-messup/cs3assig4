#ifndef RABIN_KARP_H
#define RABIN_KARP_H

#include "../include/DynamicArray.h"
#include <string>
#include <cmath>

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

RabinKarp::RabinKarp(int base = 256, int prime = 101) : base(base), prime(prime) {}

const DynamicArray<int> &RabinKarp::search(const std::string &text, const std::string &pattern)
{
    occurrences.clear();

    int n = text.length();
    int m = pattern.length();

    if (m == 0 || n < m)
        return occurrences;

    int patternHash = 0;
    int textHash = 0;
    int h = 1;

    for (int i = 0; i < m - 1; i++)
    {
        h = (h * base) % prime;
    }

    for (int i = 0; i < m; i++)
    {
        patternHash = (base * patternHash + pattern[i]) % prime;
        textHash = (base * textHash + text[i]) % prime;
    }

    for (int i = 0; i <= n - m; i++)
    {
        if (patternHash == textHash)
        {
            bool match = true;
            for (int j = 0; j < m; j++)
            {
                if (text[i + j] != pattern[j])
                {
                    match = false;
                    break;
                }
            }
            if (match)
            {
                occurrences.push_back(i);
            }
        }

        if (i < n - m)
        {
            textHash = (base * (textHash - text[i] * h) + text[i + m]) % prime;

            if (textHash < 0)
            {
                textHash += prime;
            }
        }
    }

    return occurrences;
}

#endif