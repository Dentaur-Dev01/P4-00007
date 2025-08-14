#pragma once
#include <string>

struct DiskStats {
    uint64_t bytes;
    double mb;
    double gb;
};

struct RamStats {
    uint64_t total;
    uint64_t available;
};

DiskStats getDiskStats(const std::string& path = "/");
RamStats getRamStats();
