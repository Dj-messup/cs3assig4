#include "Pattern.h"
using namespace std;
using namespace std::chrono;

// Constructor: remember table sizes and create tables
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

// Destructor: free tables and any allocated Work IX strings
Pattern::~Pattern()
{
    delete hashTable;
    delete linearTable;
    for (size_t i = 0; i < workIXWords.getSize(); i++)
    {
        delete[] workIXWords[i];
    }
}

// processWord: copy input→output, drop repeated hyphens, lowercase
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

// strConv: drop double-hyphens, keep alpha or single hyphen
void Pattern::strConv(char* word)
{
    int len = strlen(word);
    int j = 0;
    for (int i = 0; i < len; ++i)
    {
        // if we see "--", skip one
        if (word[i] == '-' && word[i+1] == '-')
        {
            ++i;
            continue;
        }
        // keep letters or hyphen
        if (isalpha(word[i]) || word[i] == '-')
        {
            word[j++] = tolower(word[i]);
        }
    }
    word[j] = '\0';
}

// classic bubble sort on wordCounts by count
void Pattern::sortWordCounts(DynamicArray<FreqEntry>& wordCounts, bool descending)
{
    for (size_t i = 0; i < wordCounts.getSize() - 1; i++)
    {
        for (size_t j = 0; j < wordCounts.getSize() - i - 1; j++)
        {
            bool shouldSwap = descending
                ? (wordCounts[j].count < wordCounts[j+1].count)
                : (wordCounts[j].count > wordCounts[j+1].count);

            if (shouldSwap)
            {
                FreqEntry temp = wordCounts[j];
                wordCounts[j] = wordCounts[j+1];
                wordCounts[j+1] = temp;
            }
        }
    }
}

// append 80-item list (most or least) to frequencies.txt
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

// readFile: single-pass through text. Key points:
//  - track which “work” we’re in via roman numerals
//  - sentenceCount++ on .,!,? at end of raw word
//  - strConv() → cleaned token; skip empty
//  - insert into hash or linear based on workNum
//  - collect tokens of work IX into workIXWords[]
//  - maintain wordCounts[] for global freq
//  - after loop, log summary stats & run search + freq dumps
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

    // rolling window to catch “ix” markers is OK omitted here
    while (inFile >> word)
    {
        // count sentences on final punctuation
        size_t len = strlen(word);
        if (len > 0 && (word[len-1]=='.' || word[len-1]=='!' || word[len-1]=='?'))
            sentenceCount++;

        // detect which story we’re in (I–XII) by roman numerals
        strConv(word);
        if (strcmp(word,"i")==0) workNum=1;
        else if (strcmp(word,"ii")==0) workNum=2;
        else if (strcmp(word,"iii")==0) workNum=3;
        else if (strcmp(word,"iv")==0) workNum=4;
        else if (strcmp(word,"v")==0) workNum=5;
        else if (strcmp(word,"vi")==0) workNum=6;
        else if (strcmp(word,"vii")==0) workNum=7;
        else if (strcmp(word,"viii")==0) workNum=8;
        else if (strcmp(word,"ix")==0) workNum=9;
        else if (strcmp(word,"x")==0) workNum=10;
        else if (strcmp(word,"xi")==0) workNum=11;
        else if (strcmp(word,"xii")==0) workNum=12;

        // clean token fully and skip if empty
        processWord(word, cleaned);
        if (strlen(cleaned)==0) continue;

        // send to chaining or linear table
        if (workNum>=1 && workNum<=6)
        {
            hashTable->insert(cleaned);
            chainInsertCount++;
        }
        else if (workNum>=7 && workNum<=12)
        {
            linearTable->insert(cleaned);
            linearInsertCount++;
        }

        // if story IX, store for user search
        if (workNum==9)
        {
            char* buffer = new char[256];
            strcpy(buffer, cleaned);
            workIXWords.push_back(buffer);
        }

        // tally global frequency (naïve linear scan)
        bool found = false;
        for (size_t i=0; i < wordCounts.getSize(); ++i)
        {
            if (strcmp(wordCounts[i].word, cleaned)==0)
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

    // log our table stats and sentence count
    Logger logger("logger.txt");
    logger.log(INFO, "Chaining inserts: " + to_string(chainInsertCount));
    logger.log(INFO, "Average chain length: " + to_string((double)chainInsertCount/chainSize));
    logger.log(INFO, "Linear inserts: " + to_string(linearInsertCount));
    logger.log(INFO, "Load factor: " + to_string((double)linearInsertCount/linearSize));
    logger.log(INFO, "Sentence count: " + to_string(sentenceCount));

    // time the userSearch step
    auto start = high_resolution_clock::now();
    userSearch();
    auto stop = high_resolution_clock::now();
    logger.log(INFO, "userSearch() completed in " +
        to_string(duration_cast<nanoseconds>(stop-start).count()) + " ns");

    // output frequency lists
    mostFrequent();
    leastFrequent();
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
