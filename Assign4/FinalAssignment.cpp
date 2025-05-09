#include <iostream>
#include <fstream>
#include <chrono>
#include <cstring>
#include <cstdlib>
#include "Pattern.h"
#include "Logger.h"

using namespace std;
using namespace std::chrono;

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        cerr << "Usage: " << argv[0] << " input.txt output.txt [mode]" << endl;
        cerr << "Modes: auto | menu" << endl;
        return 1;
    }

    const char* inputFile = argv[1];
    const char* outputFile = argv[2];
    const char* mode = (argc == 4) ? argv[3] : "auto";

    // Clear the output file for fresh results
    ofstream clearFile(outputFile);
    clearFile.close();

    Logger logger("logger.txt");
    Pattern analyzer(50, 119);

    if (strcmp(mode, "auto") == 0)
    {
        auto start = high_resolution_clock::now();
        analyzer.readFile();  // performs read + search + frequency tasks
        auto stop = high_resolution_clock::now();
        logger.log(INFO, "readFile() completed in " + to_string(duration_cast<nanoseconds>(stop - start).count()) + " ns");
    }
    else if (strcmp(mode, "menu") == 0)
    {
        int choice;
        do {
            cout << "\n===== Conan Doyle Text Analysis =====\n";
            cout << "1. Read file\n";
            cout << "2. Search Work IX\n";
            cout << "3. 80 most frequent words\n";
            cout << "4. 80 least frequent words\n";
            cout << "0. Exit\n";
            cout << "====================================\n";
            cout << "Enter your choice: ";
            cin >> choice;
            cin.ignore();

            auto start = high_resolution_clock::now();
            switch (choice)
            {
                case 1:
                    analyzer.readFile();  // will also call search and frequency unless you refactor
                    break;
                case 2:
                    analyzer.userSearch();
                    break;
                case 3:
                    analyzer.mostFrequent();
                    break;
                case 4:
                    analyzer.leastFrequent();
                    break;
                case 0:
                    cout << "Exiting program...\n";
                    break;
                default:
                    cout << "Invalid choice. Try again.\n";
            }
            auto stop = high_resolution_clock::now();
            logger.log(INFO, "Task " + to_string(choice) + " completed in " + to_string(duration_cast<nanoseconds>(stop - start).count()) + " ns");

        } while (choice != 0);
    }
    else
    {
        cerr << "Unknown mode: " << mode << endl;
        return 1;
    }

    return 0;
}