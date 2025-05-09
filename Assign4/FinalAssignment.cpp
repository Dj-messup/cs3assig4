#include <iostream>
#include <fstream>
#include <chrono>
#include <cstring>
#include <cstdlib>
#include "Pattern.h"
#include "Logger.h"

using namespace std;
using namespace chrono;

// Entry-point: expects input file, output file, optional mode
int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        cerr << "Usage: " << argv[0] << " input.txt output.txt [auto|menu]\n";
        return 1;
    }

    const char* inFile  = argv[1];
    const char* outFile = argv[2];
    const char* mode    = (argc==4) ? argv[3] : "auto";

    // clear the output file so we start fresh
    ofstream clearF(outFile); clearF.close();

    Logger logger("logger.txt");
    Pattern analyzer(50, 119);  // initial table sizes

    // auto = run everything at once, menu = interactive
    if (!strcmp(mode,"auto"))
    {
        auto start = high_resolution_clock::now();
        analyzer.readFile();
        auto end = high_resolution_clock::now();
        logger.log(INFO, "readFile() completed in "
                   + to_string(duration_cast<nanoseconds>(end-start).count()) + " ns");
    }
    else
    {
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
                case 1: analyzer.readFile(); break;
                case 2: analyzer.userSearch(); break;
                case 3: analyzer.mostFrequent(); break;
                case 4: analyzer.leastFrequent(); break;
                case 0: cout<<"Bye!\n"; break;
                default: cout<<"Invalid\n";
            }
            auto e = high_resolution_clock::now();
            logger.log(INFO, "Task " + to_string(choice) + " completed in "
                       + to_string(duration_cast<nanoseconds>(e-s).count()) + " ns");
        } while (choice!=0);
    }

    return 0;
}