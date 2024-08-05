#include "Logger.h"
#include <iostream>
#include <chrono>
#include <iomanip>

Logger::Logger() : running(false) {}

Logger::~Logger() {
    if (running) {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            running = false;
            queueCV.notify_all();
        }
        logThread.join();
    }
}

void Logger::start() {
    running = true;
    logThread = std::thread(&Logger::processQueue, this);
}

void Logger::log(const std::string& message) {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        logQueue.push(message);
    }
    queueCV.notify_one();
}

void Logger::processQueue() {
    while (running || !logQueue.empty()) {
        std::string message;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            queueCV.wait(lock, [this] { return !logQueue.empty() || !running; });

            if (!running && logQueue.empty()) {
                return;
            }

            message = logQueue.front();
            logQueue.pop();
        }
        std::cout << message << std::endl;
    }
}
