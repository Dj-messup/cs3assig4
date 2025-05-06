#ifndef LOGGER
#define LOGGER

#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <ctime>

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
};

std::string Logger::levelAsString(Level level)
{
    switch (level)
    {
    case DEBUG:
        return "DEBUG";
    case INFO:
        return "INFO";
    case WARN:
        return "WARN";
    case ERROR:
        return "ERROR";
    case FATAL:
        return "FATAL";
    default:
        return "UNKOWN";
    }
}

Logger::Logger(const std::string &file_name)
{
    log_file.open(file_name, std::ios::app);
    if (!log_file.is_open())
    {
        std::cerr << "Error opening log file" << std::endl;
    }
}

Logger::~Logger()
{
    log_file.close();
}

void Logger::log(Level level, const std::string &message)
{
    if (log_file.is_open())
    {
        auto now = std::chrono::system_clock::now();
        auto now_time = std::chrono::system_clock::to_time_t(now);
        log_file << "[" << std::put_time(std::localtime(&now_time), "%F %T") << "] "
                 << "[" << levelAsString(level) + "] "
                 << message << "\n";
        log_file.flush();
    }
}

#endif