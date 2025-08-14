# Disk and RAM Usage Daemon

This C++ console daemon records disk space and RAM usage on Ubuntu Linux to a text-based log file, with configurable interval and log rotation.

## Features
- Configurable interval (seconds) for recording stats (default: 5 seconds)
- Configurable number of log records to keep (default: 6)
- Records disk space in GB, MB, and bytes
- Records RAM usage
- Log rotation (overwrites oldest records)
- Daemon can be started and stopped

## Files
- `daemon.cpp` – Main application
- `config.ini` – Configuration file
- `logger.h`/`logger.cpp` – Log management
- `system_stats.h`/`system_stats.cpp` – System stats collection
- `daemon.h` – Daemonization helpers

## Installation
1. **Install build tools:**
   ```sh
   sudo apt update
   sudo apt install build-essential g++
   ```
2. **Build the daemon:**
   ```sh
   g++ daemon.cpp logger.cpp system_stats.cpp -o diskramd -std=c++17
   ```
3. **Edit the config file (optional):**
   - `interval_seconds`: How often to record stats (default: 5)
   - `max_records`: Number of records to keep in the log (default: 6)

## Usage
- **Start the daemon:**
  ```sh
  sudo ./diskramd start
  ```
- **Stop the daemon:**
  ```sh
  sudo ./diskramd stop
  ```
- **Log file:**
  - `diskramd.log` in the same directory
- **Config file:**
  - `config.ini` in the same directory

## Daemon Setup (Systemd)
1. **Create a systemd service file `/etc/systemd/system/diskramd.service`:**
   ```ini
   [Unit]
   Description=Disk and RAM Usage Daemon

   [Service]
   ExecStart=/path/to/diskramd start
   ExecStop=/path/to/diskramd stop
   Restart=always
   User=root
   WorkingDirectory=/path/to/

   [Install]
   WantedBy=multi-user.target
   ```
2. **Enable and start the service:**
   ```sh
   sudo systemctl daemon-reload
   sudo systemctl enable diskramd
   sudo systemctl start diskramd
   ```
3. **Stop the service:**
   ```sh
   sudo systemctl stop diskramd
   ```

## Notes
- Requires root privileges to access some system stats.
- Tested on Ubuntu.
