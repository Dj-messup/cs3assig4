// FinalAssignment.cpp

#include <iostream>
#include <fstream>
#include <chrono>
#include <cstring>
#include "Pattern.h"
#include "Logger.h"

using namespace std;
using namespace chrono;

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        cerr << "Usage: " << argv[0]
             << " input.txt output.txt [auto|menu]\n";
        return 1;
    }

    const char* inFile  = argv[1];
    const char* outFile = argv[2];
    const char* mode    = (argc == 4) ? argv[3] : "auto";

    // clear output
    ofstream clearF(outFile);
    clearF.close();

    Logger logger("logger.txt");
    Pattern analyzer(50, 119);

    if (!strcmp(mode, "auto"))
    {
        auto start = high_resolution_clock::now();
        analyzer.readFile(inFile);
        auto end = high_resolution_clock::now();
        logger.log(INFO, "readFile() completed in "
                   + to_string(duration_cast<nanoseconds>(end - start).count())
                   + " ns");
    }
    else
    {
        bool loaded = false;
        int choice;

        do {
            cout << "\n==== Conan Doyle Analysis ====\n"
                 << "1. Read file\n"
                 << "2. Search Work IX\n"
                 << "3. 80 most frequent words\n"
                 << "4. 80 least frequent words\n"
                 << "0. Exit\n"
                 << "==============================\n"
                 << "Choice: ";
            cin >> choice; cin.ignore();

            auto s = high_resolution_clock::now();
            switch (choice)
            {
                case 1:
                    analyzer.readFile(inFile);
                    loaded = true;
                    break;
                case 2:
                    if (!loaded) cout << "Please load the file first (1)\n";
                    else         analyzer.userSearch();
                    break;
                case 3:
                    if (!loaded) cout << "Please load the file first (1)\n";
                    else         analyzer.mostFrequent();
                    break;
                case 4:
                    if (!loaded) cout << "Please load the file first (1)\n";
                    else         analyzer.leastFrequent();
                    break;
                case 0:
                    cout << "Bye!\n";
                    break;
                default:
                    cout << "Invalid choice\n";
            }
            auto e = high_resolution_clock::now();
            logger.log(INFO, "Task " + to_string(choice)
                       + " completed in "
                       + to_string(duration_cast<nanoseconds>(e - s).count())
                       + " ns");
        } while (choice != 0);
    }

    return 0;
}
