#include <iostream>
#include <fstream>
#include <chrono>
#include "Pattern.h"
#include "Logger.h"

using namespace std;
using namespace std::chrono;

void displayMenu()
{
    cout << "\n===== Conan Doyle Text Analysis =====\n";
    cout << "1. Search for words in 'The Adventure of the Engineer's Thumb'\n";
    cout << "2. Display 80 most frequent words\n";
    cout << "3. Display 80 least frequent words\n";
    cout << "0. Exit\n";
    cout << "====================================\n";
    cout << "Enter your choice: ";
}

void processMenuChoice(Pattern &analyzer, Logger &logger)
{
    int choice;
    cin >> choice;
    cin.ignore();

    auto start = high_resolution_clock::now();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop - start);

    switch (choice)
    {
    case 1:
    {
        start = high_resolution_clock::now();
        analyzer.userSearch();
        stop = high_resolution_clock::now();
        duration = duration_cast<nanoseconds>(stop - start);

        logger.log(INFO, "Search completed in " + to_string(duration.count()) + " ns");
        break;
    }

    case 2:
    {
        start = high_resolution_clock::now();
        analyzer.mostFrequent();
        stop = high_resolution_clock::now();
        duration = duration_cast<nanoseconds>(stop - start);

        logger.log(INFO, "Most frequent words listed in " + to_string(duration.count()) + " ns");
        cout << "80 most frequent words saved to frequencies.txt\n";
        break;
    }

    case 3:
    {
        start = high_resolution_clock::now();
        analyzer.leastFrequent();
        stop = high_resolution_clock::now();
        duration = duration_cast<nanoseconds>(stop - start);

        logger.log(INFO, "Least frequent words listed in " + to_string(duration.count()) + " ns");
        cout << "80 least frequent words saved to frequencies.txt\n";
        break;
    }

    case 0:
        cout << "Exiting program...\n";
        exit(0);

    default:
        cout << "Invalid choice. Please try again.\n";
    }
}

int main(int argc, char *argv[])
{
    Logger logger("logger.txt");
    Pattern analyzer(1000);

    try
    {
        while (true)
        {
            displayMenu();
            processMenuChoice(analyzer, logger);
        }
    }
    catch (const exception &e)
    {
        logger.log(ERROR, string("Fatal error: ") + e.what());
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}