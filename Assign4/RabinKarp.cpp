#include "RabinKarp.h"

//base and modulus
RabinKarp::RabinKarp(int base, int prime) : base(base), prime(prime) {}

// Search returns a list of all starting indices where pattern matches text
const DynamicArray<int> &RabinKarp::search(const std::string &text, const std::string &pattern)
{
    occurrences.clear();
    int n = text.length(), m = pattern.length();
    if (m == 0 || n < m)
        return occurrences;

    int patternHash = 0, textHash = 0, h = 1;

    // precompute h = base^(m-1) % prime
    for (int i = 0; i < m - 1; i++)

        h = (h * base) % prime;


    // initial hash values
    for (int i = 0; i < m; i++)
    {
        patternHash = (base * patternHash + pattern[i]) % prime;
        textHash = (base * textHash + text[i]) % prime;
    }

    // slide window
    for (int i = 0; i <= n - m; i++)
    {
        if (patternHash == textHash)
        {
            // double-check
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
                occurrences.push_back(i);
        }
        // compute hash for next window
        if (i < n - m)
        {
            textHash = (base * (textHash - text[i] * h) + text[i + m]) % prime;

            if (textHash < 0)

                textHash += prime;

        }
    }

    return occurrences;
}