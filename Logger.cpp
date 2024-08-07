#include "Logger.h"
#include <chrono>
#include <iomanip>

void Logger::logBankOpened() {
    log(getCurrentTime() + " Bank opened");
}

void Logger::logBankClosed() {
    log(getCurrentTime() + " Bank closed");
}

void Logger::logClientArrived(const std::string& clientName, const std::string& departmentName) {
    log(getCurrentTime() + " Client " + clientName + " came to the department " + departmentName);
}

void Logger::logClientServed(const std::string& clientName, const std::string& departmentName) {
    log(getCurrentTime() + " Client " + clientName + " is served by the department " + departmentName);
}

void Logger::logClientLeft(const std::string& clientName, const std::string& departmentName) {
    log(getCurrentTime() + " Client " + clientName + " left the department " + departmentName);
}

void Logger::logClientDone(const std::string& clientName) {
    log(getCurrentTime() + " Client " + clientName + " left the bank");
}

void Logger::log(const std::string& message) {
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << message << std::endl;
}

std::string Logger::getCurrentTime() const {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    std::ostringstream oss;
    oss << "[" << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S.") << std::setw(3) << std::setfill('0') << ms.count() << "]";
    return oss.str();
}