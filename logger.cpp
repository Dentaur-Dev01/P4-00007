#include "logger.h"
#include <fstream>
#include <vector>

Logger::Logger(const std::string& filename, int maxRecords)
    : filename(filename), maxRecords(maxRecords) {}

void Logger::log(const std::string& record) {
    auto records = readAll();
    if ((int)records.size() >= maxRecords) {
        records.erase(records.begin());
    }
    records.push_back(record);
    std::ofstream ofs(filename, std::ios::trunc);
    for (const auto& r : records) {
        ofs << r << std::endl;
    }
}

std::vector<std::string> Logger::readAll() {
    std::vector<std::string> records;
    std::ifstream ifs(filename);
    std::string line;
    while (std::getline(ifs, line)) {
        if (!line.empty()) records.push_back(line);
    }
    return records;
}
