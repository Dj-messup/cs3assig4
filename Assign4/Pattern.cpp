#include "Pattern.h"
using namespace std;
using namespace std::chrono;

Pattern::Pattern(int chainSize, int linearSize)
  : workNum(0),
    chainInsertCount(0),
    linearInsertCount(0),
    sentenceCount(0),
    chainSize(chainSize),
    linearSize(linearSize)
{
    hashTable   = new HashTable(chainSize);
    linearTable = new LinearTable(linearSize);
}

Pattern::~Pattern()
{
    delete hashTable;
    delete linearTable;
    for (size_t i = 0; i < workIXWords.getSize(); ++i)
        delete[] workIXWords[i];
}

void Pattern::processWord(const char* input, char* output)
{
    int out_pos = 0;
    bool lastHyphen = false;
    for (int i = 0; input[i] && out_pos < 255; ++i)
    {
        char c = tolower(input[i]);
        if (c == '-')
        {
            if (!lastHyphen)
            {
                output[out_pos++] = c;
                lastHyphen = true;
            }
        }
        else
        {
            output[out_pos++] = c;
            lastHyphen = false;
        }
    }
    output[out_pos] = '\0';
}

void Pattern::strConv(char* word)
{
    int len = strlen(word), j = 0;
    for (int i = 0; i < len; ++i)
    {
        if (word[i] == '-' && word[i+1] == '-')
        {
            ++i;
            continue;
        }
        if (isalpha(word[i]) || word[i] == '-')
            word[j++] = tolower(word[i]);
    }
    word[j] = '\0';
}

void Pattern::sortWordCounts(DynamicArray<FreqEntry>& arr, bool desc)
{
    size_t n = arr.getSize();
    if (n < 2) return;
    for (size_t i = 0; i < n - 1; ++i)
    {
        for (size_t j = 0; j < n - i - 1; ++j)
        {
            bool swapCond = desc
                ? (arr[j].count < arr[j+1].count)
                : (arr[j].count > arr[j+1].count);
            if (swapCond)
                std::swap(arr[j], arr[j+1]);
        }
    }
}

void Pattern::outputFrequencyList(DynamicArray<FreqEntry>& arr,
                                  const char* filename,
                                  const char* title)
{
    ofstream out(filename, ios::app);
    if (!out.is_open()) return;

    out << "\n=== " << title << " ===\n";
    out << "Word\t\tFrequency\n-----------------------\n";

    size_t limit = arr.getSize() > 80 ? 80 : arr.getSize();
    for (size_t i = 0; i < limit; ++i)
        out << arr[i].word << "\t\t" << arr[i].count << "\n";
}

void Pattern::readFile(const char* filename)
{
    ifstream inFile(filename);
    if (!inFile)
    {
        cerr << "Error: cannot open " << filename << "\n";
        exit(1);
    }

    // reset
    workNum            = 0;
    chainInsertCount   = linearInsertCount = sentenceCount = 0;
    wordCounts.clear();
    workIXWords.clear();

    char raw[256], cleaned[256];
    while (inFile >> raw)
    {
        // sentence count on ., !, ?
        size_t len = strlen(raw);
        if (len && (raw[len-1]=='.' || raw[len-1]=='!' || raw[len-1]=='?'))
            ++sentenceCount;

        // normalize for heading detection
        strConv(raw);

        // flip into IX when you see the IX heading
        if (strcmp(raw, "ix") == 0)
        {
            workNum = 9;
            continue;    // skip the "ix" token itself
        }
        // once you hit X, you leave story IX
        if (strcmp(raw, "x") == 0 && workNum == 9)
        {
            workNum = 10;
        }

        // clean & skip empties
        processWord(raw, cleaned);
        if (!strlen(cleaned)) continue;

        // route into hash or linear as before
        if (workNum >= 1 && workNum <= 6)
        {
            hashTable->insert(cleaned);
            ++chainInsertCount;
        }
        else if (workNum >= 7 && workNum <= 12)
        {
            linearTable->insert(cleaned);
            ++linearInsertCount;
        }

        // capture only story IX tokens
        if (workNum == 9)
        {
            char* buf = new char[256];
            strcpy(buf, cleaned);
            workIXWords.push_back(buf);
        }

        // global frequency tally
        bool found = false;
        for (size_t i = 0; i < wordCounts.getSize(); ++i)
        {
            if (!strcmp(wordCounts[i].word, cleaned))
            {
                ++wordCounts[i].count;
                found = true;
                break;
            }
        }
        if (!found)
            wordCounts.push_back(FreqEntry(cleaned));
    }
    inFile.close();

    // log stats
    Logger logger("logger.txt");
    logger.log(INFO, "Chaining inserts: "    + to_string(chainInsertCount));
    logger.log(INFO, "Average chain length: "+ to_string((double)chainInsertCount/chainSize));
    logger.log(INFO, "Linear inserts: "      + to_string(linearInsertCount));
    logger.log(INFO, "Load factor: "         + to_string((double)linearInsertCount/linearSize));
    logger.log(INFO, "Sentence count: "      + to_string(sentenceCount));
}

// userSearch: prompt for up to 8 keys, split on '@@@', lowercase,
// then scan workIXWords linearly and report word positions
void Pattern::userSearch()
{
    cout << "Enter up to 8 search keys, separated by '@@@' if less than 8: ";
    char inputLine[2048];
    cin.getline(inputLine, 2048);

    char* tokens[8];
    int count=0;
    char* token = strtok(inputLine, "@@@");
    while (token!=NULL && count<8)
    {
        tokens[count++] = token;
        token = strtok(NULL, "@@@");
    }

    for (int k=0; k<count; k++)
    {
        // lowercase the pattern
        char lowered[256];
        int i=0;
        while (tokens[k][i])
        {
            lowered[i] = tolower(tokens[k][i]);
            i++;
        }
        lowered[i]='\0';

        cout << "\nPattern: '" << lowered << "' found at word positions: ";
        bool found=false;
        // brute-force scan
        for (size_t i=0; i<workIXWords.getSize(); i++)
        {
            if (strcmp(workIXWords[i], lowered)==0)
            {
                if (found) cout << ", ";
                cout << (i+1);
                found=true;
            }
        }

        if (!found) cout << "Not found";
        cout << endl;
    }
}

// grab a copy, sort descending, write top 80
void Pattern::mostFrequent()
{
    DynamicArray<FreqEntry> most = wordCounts;
    sortWordCounts(most, true);
    outputFrequencyList(most, "frequencies.txt", "80 Most Frequent Words");
}

// copy, sort ascending, write bottom 80
void Pattern::leastFrequent()
{
    DynamicArray<FreqEntry> least = wordCounts;
    sortWordCounts(least, false);
    outputFrequencyList(least, "frequencies.txt", "80 Least Frequent Words");
}

// combined count from both tables if needed
int Pattern::getCount(const char* word)
{
    int count=0;
    if (hashTable) count += hashTable->getCount(word);
    if (linearTable) count += linearTable->getCount(word);
    return count;
}
