#include "system_stats.h"
#include <sys/statvfs.h>
#include <fstream>
#include <sstream>

DiskStats getDiskStats(const std::string& path) {
    struct statvfs stat;
    DiskStats stats{0, 0, 0};
    if (statvfs(path.c_str(), &stat) == 0) {
        uint64_t bytes = stat.f_bsize * stat.f_bavail;
        stats.bytes = bytes;
        stats.mb = bytes / (1024.0 * 1024.0);
        stats.gb = bytes / (1024.0 * 1024.0 * 1024.0);
    }
    return stats;
}

RamStats getRamStats() {
    std::ifstream meminfo("/proc/meminfo");
    std::string line;
    RamStats stats{0, 0};
    while (std::getline(meminfo, line)) {
        std::istringstream iss(line);
        std::string key;
        uint64_t value;
        std::string unit;
        iss >> key >> value >> unit;
        if (key == "MemTotal:") stats.total = value * 1024;
        if (key == "MemAvailable:") stats.available = value * 1024;
    }
    return stats;
}
