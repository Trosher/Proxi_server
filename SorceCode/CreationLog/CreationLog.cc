#include "CreationLog.h"

Logger::Logger(const char* path) {
    file_.open(path, std::fstream::app);
    if (!file_.is_open()) {
        std::cout << "ERROR: It is impossible to open a file with logs" << std::endl;
    }
}

Logger::~Logger() { 
    file_.close(); 
}

void Logger::CreateLog(const char* buf) {
    if (file_.is_open()) {
        auto time_data_now = std::chrono::system_clock::now();
        std::time_t time_data_end = std::chrono::system_clock::to_time_t(time_data_now);
        file_ 
            << "SQL reuest: "
            << std::ctime(&time_data_end)
            << std::endl
            << buf
            << "\n\n";
    } else {
        std::cout << "ERROR: It is impossible to write logs to a file\n" << std::endl;
    }
}