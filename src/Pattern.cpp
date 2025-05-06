#include "Pattern.h"

using namespace std;

void Pattern::processWord(const char *input, char *output)
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

void Pattern::countWordFrequencies(const char *filename, DynamicArray<WordEntry> &wordCounts)
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

void Pattern::sortWordCounts(DynamicArray<WordEntry> &wordCounts, bool descending)
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

void Pattern::outputFrequencyList(DynamicArray<WordEntry> &wordCounts, const char *filename, const char *title)
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

Pattern::Pattern()
{
    hashTable = nullptr;
    linearTable = nullptr;
    workNum = 0;
    countWordFrequencies("A_Scandal_In_Bohemia.txt", wordCounts);
}

Pattern::Pattern(int tableSize)
{
    hashTable = new HashTable(tableSize);
    linearTable = new LinearTable(tableSize);
    workNum = 0;
    countWordFrequencies("A_Scandal_In_Bohemia.txt", wordCounts);
}

Pattern::~Pattern()
{
    delete hashTable;
    delete linearTable;
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
    std::ifstream inFile("A Scandal In Bohemia.txt");
    if (!inFile)
    {
        std::cerr << "Error: File could not be opened." << std::endl;
        exit(1);
    }

    char word[256];
    while (inFile >> word)
    {
        strConv(word);

        if (strlen(word) == 1 && word[0] >= 'i' && word[0] <= 'x')
        {
            workNum++;
            continue;
        }

        if (strlen(word) == 0)
            continue;

        if (workNum >= 1 && workNum <= 6)
        {
            hashTable->insert(word);
        }
        else if (workNum >= 7 && workNum <= 12)
        {
            linearTable->insert(word);
        }
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