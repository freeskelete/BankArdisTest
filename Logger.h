#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>

class Logger {
public:
    Logger();
    ~Logger();
    void start();
    void log(const std::string& message);

private:
    void processQueue();
    std::queue<std::string> logQueue;
    std::mutex queueMutex;
    std::condition_variable queueCV;
    std::thread logThread;
    bool running;
};

#endif // LOGGER_H
