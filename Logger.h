#ifndef LOGGER_H
#define LOGGER_H

#include <mutex>
#include <iostream>
#include <sstream>

class Logger {
private:
    void log(const std::string& message);
    std::mutex mtx;
    std::string getCurrentTime() const;    
public:
    void logBankOpened();
    void logBankClosed();
    void logClientArrived(const std::string& clientName, const std::string& departmentName);
    void logClientServed(const std::string& clientName, const std::string& departmentName);
    void logClientLeft(const std::string& clientName, const std::string& departmentName);
    void logClientDone(const std::string& clientName);
};

#endif // LOGGER_H
