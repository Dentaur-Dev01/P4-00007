#pragma once
#include <string>
#include <vector>

class Logger {
public:
    Logger(const std::string& filename, int maxRecords);
    void log(const std::string& record);
private:
    std::string filename;
    int maxRecords;
    void rotate();
    std::vector<std::string> readAll();
};
