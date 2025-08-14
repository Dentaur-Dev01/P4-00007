#pragma once
void daemonize();
void writePidFile(const char* pidfile);
void removePidFile(const char* pidfile);
bool isRunning(const char* pidfile);
