#include "Pattern.h"
using namespace std;
using namespace std::chrono;

Pattern::Pattern(int chainSize, int linearSize)
{
    this->chainSize = chainSize;
    this->linearSize = linearSize;
    hashTable = new HashTable(chainSize);
    linearTable = new LinearTable(linearSize);
    workNum = 0;
    chainInsertCount = 0;
    linearInsertCount = 0;
    sentenceCount = 0;
}

Pattern::~Pattern()
{
    delete hashTable;
    delete linearTable;
    for (size_t i = 0; i < workIXWords.getSize(); i++)
    {
        delete[] workIXWords[i];
    }
}

void Pattern::processWord(const char* input, char* output)
{
    int out_pos = 0;
    bool last_was_hyphen = false;
    for (int i = 0; input[i] != '\0' && out_pos < 255; i++)
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

void Pattern::strConv(char* word)
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

void Pattern::sortWordCounts(DynamicArray<FreqEntry>& wordCounts, bool descending)
{
    for (size_t i = 0; i < wordCounts.getSize() - 1; i++)
    {
        for (size_t j = 0; j < wordCounts.getSize() - i - 1; j++)
        {
            bool shouldSwap = descending
                ? (wordCounts[j].count < wordCounts[j + 1].count)
                : (wordCounts[j].count > wordCounts[j + 1].count);

            if (shouldSwap)
            {
                FreqEntry temp = wordCounts[j];
                wordCounts[j] = wordCounts[j + 1];
                wordCounts[j + 1] = temp;
            }
        }
    }
}

void Pattern::outputFrequencyList(DynamicArray<FreqEntry>& wordCounts, const char* filename, const char* title)
{
    ofstream outputFile(filename, ios::app);
    if (!outputFile.is_open()) return;

    outputFile << "\n=== " << title << " ===\n";
    outputFile << "Word\t\tFrequency\n";
    outputFile << "-----------------------\n";

    size_t limit = wordCounts.getSize() > 80 ? 80 : wordCounts.getSize();
    for (size_t i = 0; i < limit; i++)
    {
        outputFile << wordCounts[i].word << "\t\t" << wordCounts[i].count << "\n";
    }

    outputFile.close();
}

void Pattern::readFile()
{
    ifstream inFile("A_Scandal_In_Bohemia.txt");
    if (!inFile)
    {
        cerr << "Error: File could not be opened." << endl;
        exit(1);
    }

    char word[256], cleaned[256];
    char w1[256] = "", w2[256] = "", w3[256] = "", w4[256] = "", w5[256] = "", w6[256] = "";

    while (inFile >> word)
    {
        strcpy(w1, w2); strcpy(w2, w3); strcpy(w3, w4);
        strcpy(w4, w5); strcpy(w5, w6); strcpy(w6, word);

        size_t len = strlen(word);
        if (len > 0 && (word[len - 1] == '.' || word[len - 1] == '!' || word[len - 1] == '?'))
            sentenceCount++;

        strConv(word);
        if (strlen(word) == 0) continue;

        if (strcmp(word, "i") == 0) workNum = 1;
        else if (strcmp(word, "ii") == 0) workNum = 2;
        else if (strcmp(word, "iii") == 0) workNum = 3;
        else if (strcmp(word, "iv") == 0) workNum = 4;
        else if (strcmp(word, "v") == 0) workNum = 5;
        else if (strcmp(word, "vi") == 0) workNum = 6;
        else if (strcmp(word, "vii") == 0) workNum = 7;
        else if (strcmp(word, "viii") == 0) workNum = 8;
        else if (strcmp(word, "ix") == 0) workNum = 9;
        else if (strcmp(word, "x") == 0) workNum = 10;
        else if (strcmp(word, "xi") == 0) workNum = 11;
        else if (strcmp(word, "xii") == 0) workNum = 12;

        processWord(word, cleaned);
        if (strlen(cleaned) == 0) continue;

        if (workNum >= 1 && workNum <= 6)
        {
            hashTable->insert(cleaned);
            chainInsertCount++;
        }
        else if (workNum >= 7 && workNum <= 12)
        {
            linearTable->insert(cleaned);
            linearInsertCount++;
        }

        if (workNum == 9)
        {
            char* buffer = new char[256];
            strcpy(buffer, cleaned);
            workIXWords.push_back(buffer);
        }

        bool found = false;
        for (size_t i = 0; i < wordCounts.getSize(); ++i)
        {
            if (strcmp(wordCounts[i].word, cleaned) == 0)
            {
                wordCounts[i].count++;
                found = true;
                break;
            }
        }
        if (!found)
        {
            FreqEntry entry(cleaned);
            wordCounts.push_back(entry);
        }
    }

    inFile.close();

    Logger logger("logger.txt");
    logger.log(INFO, "Chaining inserts: " + to_string(chainInsertCount));
    logger.log(INFO, "Average chain length: " + to_string((double)chainInsertCount / chainSize));
    logger.log(INFO, "Linear inserts: " + to_string(linearInsertCount));
    logger.log(INFO, "Load factor: " + to_string((double)linearInsertCount / linearSize));
    logger.log(INFO, "Sentence count: " + to_string(sentenceCount));

    auto start = high_resolution_clock::now();
    userSearch();
    auto stop = high_resolution_clock::now();
    logger.log(INFO, "userSearch() completed in " + to_string(duration_cast<nanoseconds>(stop - start).count()) + " ns");

    mostFrequent();
    leastFrequent();
}

void Pattern::userSearch()
{
    cout << "Enter up to 8 search keys, separated by '@@@' if less than 8: ";
    char inputLine[2048];
    cin.getline(inputLine, 2048);

    char* tokens[8];
    int count = 0;
    char* token = strtok(inputLine, "@@@");
    while (token != NULL && count < 8)
    {
        tokens[count++] = token;
        token = strtok(NULL, "@@@");
    }

    RabinKarp rk;

    for (int k = 0; k < count; k++)
    {
        char lowered[256];
        int i = 0;
        while (tokens[k][i])
        {
            lowered[i] = tolower(tokens[k][i]);
            i++;
        }
        lowered[i] = '\0';

        cout << "\nPattern: '" << lowered << "' found at word positions: ";
        bool found = false;

        for (size_t i = 0; i < workIXWords.getSize(); i++)
        {
            if (strcmp(workIXWords[i], lowered) == 0)
            {
                if (found) cout << ", ";
                cout << (i + 1);
                found = true;
            }
        }

        if (!found) cout << "Not found";
        cout << endl;
    }
}

void Pattern::mostFrequent()
{
    DynamicArray<FreqEntry> most = wordCounts;
    sortWordCounts(most, true);
    outputFrequencyList(most, "frequencies.txt", "80 Most Frequent Words");
}

void Pattern::leastFrequent()
{
    DynamicArray<FreqEntry> least = wordCounts;
    sortWordCounts(least, false);
    outputFrequencyList(least, "frequencies.txt", "80 Least Frequent Words");
}

int Pattern::getCount(const char* word)
{
    int count = 0;
    if (hashTable) count += hashTable->getCount(word);
    if (linearTable) count += linearTable->getCount(word);
    return count;
}