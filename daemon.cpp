#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <csignal>
#include <atomic>
#include "logger.h"
#include "system_stats.h"
#include "daemon.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

const char* CONFIG_FILE = "config.ini";
const char* LOG_FILE = "diskramd.log";
const char* PID_FILE = "diskramd.pid";
std::atomic<bool> running(true);

void signalHandler(int) {
    running = false;
}

struct Config {
    int interval = 5;
    int maxRecords = 6;
};

Config readConfig() {
    Config cfg;
    std::ifstream ifs(CONFIG_FILE);
    std::string line;
    while (std::getline(ifs, line)) {
        if (line.find("interval_seconds=") == 0)
            cfg.interval = std::stoi(line.substr(17));
        else if (line.find("max_records=") == 0)
            cfg.maxRecords = std::stoi(line.substr(12));
    }
    return cfg;
}

void runDaemon(const Config& cfg) {
    Logger logger(LOG_FILE, cfg.maxRecords);
    while (running) {
        auto disk = getDiskStats();
        auto ram = getRamStats();
        std::ostringstream oss;
        oss << "DISK: " << disk.gb << " GB, " << disk.mb << " MB, " << disk.bytes << " bytes | RAM: "
            << ram.available << "/" << ram.total << " bytes available/total | "
            << std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        logger.log(oss.str());
        for (int i = 0; i < cfg.interval && running; ++i)
            std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void daemonize() {
    pid_t pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);
    umask(0);
    setsid();
    chdir("/");
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}

void writePidFile(const char* pidfile) {
    std::ofstream ofs(pidfile);
    ofs << getpid();
}

void removePidFile(const char* pidfile) {
    unlink(pidfile);
}

bool isRunning(const char* pidfile) {
    std::ifstream ifs(pidfile);
    pid_t pid;
    if (!(ifs >> pid)) return false;
    return (kill(pid, 0) == 0);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " [start|stop]" << std::endl;
        return 1;
    }
    if (std::string(argv[1]) == "start") {
        if (isRunning(PID_FILE)) {
            std::cout << "Daemon already running." << std::endl;
            return 1;
        }
        daemonize();
        writePidFile(PID_FILE);
        std::signal(SIGTERM, signalHandler);
        std::signal(SIGINT, signalHandler);
        auto cfg = readConfig();
        runDaemon(cfg);
        removePidFile(PID_FILE);
    } else if (std::string(argv[1]) == "stop") {
        std::ifstream ifs(PID_FILE);
        pid_t pid;
        if (!(ifs >> pid)) {
            std::cout << "No running daemon found." << std::endl;
            return 1;
        }
        kill(pid, SIGTERM);
        std::cout << "Daemon stopped." << std::endl;
    } else {
        std::cout << "Unknown command." << std::endl;
        return 1;
    }
    return 0;
}
