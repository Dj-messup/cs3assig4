#include <iostream>
#include <fstream>
#include <chrono>
#include <cstring>
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

void processMenuChoice(Pattern &analyzer, Logger &logger, int choice)
{
    auto start = high_resolution_clock::now();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop - start);

    switch (choice)
    {
    case 1:
        start = high_resolution_clock::now();
        analyzer.userSearch();
        stop = high_resolution_clock::now();
        duration = duration_cast<nanoseconds>(stop - start);
        logger.log(INFO, "userSearch() completed in " + to_string(duration.count()) + " ns");
        break;

    case 2:
        start = high_resolution_clock::now();
        analyzer.mostFrequent();
        stop = high_resolution_clock::now();
        duration = duration_cast<nanoseconds>(stop - start);
        logger.log(INFO, "mostFrequent() completed in " + to_string(duration.count()) + " ns");
        cout << "80 most frequent words saved to frequencies.txt\n";
        break;

    case 3:
        start = high_resolution_clock::now();
        analyzer.leastFrequent();
        stop = high_resolution_clock::now();
        duration = duration_cast<nanoseconds>(stop - start);
        logger.log(INFO, "leastFrequent() completed in " + to_string(duration.count()) + " ns");
        cout << "80 least frequent words saved to frequencies.txt\n";
        break;

    default:
        cout << "Invalid choice. Please try again.\n";
        break;
    }
}

int main()
{
    Logger logger("logger.txt");

    Pattern analyzer(50, 119);

    auto start = high_resolution_clock::now();
    analyzer.readFile();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop - start);
    logger.log(INFO, "readFile() completed in " + to_string(duration.count()) + " ns");

    while (true)
    {
        displayMenu();
        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 0)
        {
            cout << "Exiting program...\n";
            break;
        }

        processMenuChoice(analyzer, logger, choice);
    }

    return 0;
}