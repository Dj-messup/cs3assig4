#include "Pattern.h"
using namespace std;

void Pattern::processWord(const char *input, char *output)
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

void Pattern::countWordFrequencies(const char *filename, DynamicArray<FreqEntry> &wordCounts)
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

            FreqEntry wf(processed);

            wordCounts.push_back(wf);
        }
    }

    inputFile.close();
}

void Pattern::sortWordCounts(DynamicArray<FreqEntry> &wordCounts, bool descending)
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

void Pattern::outputFrequencyList(DynamicArray<FreqEntry> &wordCounts, const char *filename, const char *title)
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

string Pattern::extractStoryIX(const string &filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        throw runtime_error("Error opening file: " + filename);
    }

    stringstream buffer;
    string line;
    bool inStoryIX = false;
    const string storyStartMarker = "IX. THE ADVENTURE OF THE ENGINEER'S THUMB";
    const string storyEndMarker = "X. THE ADVENTURE OF";

    while (getline(file, line))
    {
        if (line.find(storyStartMarker) != string::npos)
        {
            inStoryIX = true;
            continue;
        }
        if (inStoryIX && line.find(storyEndMarker) != string::npos)
        {
            break;
        }
        if (inStoryIX)
        {
            buffer << line << "\n";
        }
    }

    if (buffer.str().empty())
    {
        throw runtime_error("Story IX not found in file");
    }

    return buffer.str();
}

DynamicArray<string> Pattern::splitString(const string &input, const string &delimiter)
{
    DynamicArray<string> tokens;
    size_t start = 0;
    size_t end = input.find(delimiter);

    while (end != string::npos)
    {
        tokens.push_back(input.substr(start, end - start));
        start = end + delimiter.length();
        end = input.find(delimiter, start);
    }
    tokens.push_back(input.substr(start));
    return tokens;
}

DynamicArray<Pair<string, size_t>> Pattern::getWordsWithPositions(const string &text)
{
    DynamicArray<Pair<string, size_t>> words;
    string currentWord;
    size_t position = 0;
    bool inWord = false;

    for (size_t i = 0; i < text.length(); i++)
    {
        if (isalpha(text[i]))
        {
            if (!inWord)
            {
                position = words.getSize() + 1;
                inWord = true;
            }
            currentWord += tolower(text[i]);
        }
        else
        {
            if (inWord)
            {
                words.push_back(Pair<string, size_t>(currentWord, position));
                currentWord.clear();
                inWord = false;
            }
        }
    }

    if (!currentWord.empty())
    {
        words.push_back(Pair<string, size_t>(currentWord, position));
    }

    return words;
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
    DynamicArray<FreqEntry> leastFrequent = wordCounts;
    sortWordCounts(leastFrequent, false);
    outputFrequencyList(leastFrequent, "frequencies.txt", "80 Least Frequent Words");
}

void Pattern::mostFrequent()
{
    DynamicArray<FreqEntry> mostFrequent = wordCounts;
    sortWordCounts(mostFrequent, true);
    outputFrequencyList(mostFrequent, "frequencies.txt", "80 Most Frequent Words");
}

void Pattern::readFile()
{
    ifstream inFile("A_Scandal_In_Bohemia.txt");
    if (!inFile)
    {
        cerr << "Error: File could not be opened." << endl;
        exit(1);
    }

    char word[256];
    char processed[256];
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

        processWord(word, processed);
        if (strlen(processed) == 0)
            continue;

        bool found = false;
        for (size_t i = 0; i < wordCounts.getSize(); ++i)
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
            wordCounts.push_back(FreqEntry(processed));
        }
    }

    inFile.close();
}

void Pattern::userSearch()
{
    string text = extractStoryIX("A_Scandal_In_Bohemia.txt");
    auto wordsWithPositions = getWordsWithPositions(text);

    string searchText;
    for (size_t i = 0; i < wordsWithPositions.getSize(); i++)
    {
        searchText += wordsWithPositions[i].first + " ";
    }

    cout << "Enter up to 8 search keys, separated by '@@@' if less than 8: ";
    string input;
    getline(cin, input);

    auto patterns = splitString(input, "@@@");
    if (patterns.getSize() > 8)
    {
        cout << "Warning: Only the first 8 patterns will be used." << endl;
        DynamicArray<string> temp;
        for (size_t i = 0; i < 8; i++)
        {
            temp.push_back(patterns[i]);
        }
        patterns = temp;
    }

    RabinKarp rk;

    for (size_t p = 0; p < patterns.getSize(); p++)
    {
        if (patterns[p].empty())
            continue;

        string lowercasePattern;
        for (size_t i = 0; i < patterns[p].length(); i++)
        {
            lowercasePattern += tolower(patterns[p][i]);
        }

        const DynamicArray<int> &occurrences = rk.search(searchText, lowercasePattern);

        cout << "\nPattern: '" << patterns[p] << "' found at word positions: ";
        if (occurrences.getSize() == 0)
        {
            cout << "Not found";
        }
        else
        {
            for (int i = 0; i < occurrences.getSize(); i++)
            {
                size_t wordPos = 0;
                size_t charPos = occurrences[i];
                size_t spaceCount = 0;

                for (size_t j = 0; j < charPos; j++)
                {
                    if (searchText[j] == ' ')
                    {
                        spaceCount++;
                    }
                }
                wordPos = spaceCount + 1;

                cout << wordPos;
                if (i < occurrences.getSize() - 1)
                {
                    cout << ", ";
                }
            }
        }
        cout << endl;
    }
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

int Pattern::getCount(const char *word)
{
    int count = 0;
    if (hashTable)
        count += hashTable->getCount(word);
    if (linearTable)
        count += linearTable->getCount(word);
    return count;
}