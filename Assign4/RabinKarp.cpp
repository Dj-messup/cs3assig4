#include "RabinKarp.h"

RabinKarp::RabinKarp(int base, int prime) : base(base), prime(prime) {}

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