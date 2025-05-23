#pragma once

#include <iostream>
#include <fstream>
#include <iomanip>

enum Level
{
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL,
};

class Logger
{
private:
    std::ofstream log_file;
    std::string levelAsString(Level level);

public:
    Logger(const std::string &file_name);
    ~Logger();
    void log(Level level, const std::string &message);
    // Helper to write sentence-count section to an output file
    static void printSentenceCount(std::ofstream& file, int sentenceCount);
};
