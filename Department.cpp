#include "Department.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>

Department::Department(const std::string& name, size_t numEmployees)
    : name(name), numEmployees(numEmployees) {}

void Department::addClient(std::shared_ptr<Client> client) {
    {
        std::unique_lock<std::mutex> lock(mtx);
        clientQueue.push(client);
    }
    cv.notify_one();

    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    std::cout << "[" << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S.") << std::setw(3) << std::setfill('0') << ms.count() << "] Client " << client->getName() << " came to the department " << name << std::endl;
}

void Department::workerAvailable() {
    std::unique_lock<std::mutex> lock(mtx);
    while (clientQueue.empty()) {
        cv.wait(lock);
    }

    if (clientQueue.empty()) {
        return;
    }

    auto client = clientQueue.front();
    clientQueue.pop();

    lock.unlock(); 

    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    std::cout << "[" << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S.") << std::setw(3) << std::setfill('0') << ms.count() << "] " << client->getName() << " is served by the department " << name << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(client->getPriority() * 100)); 

    now = std::chrono::system_clock::now();
    auto serviceEndTime = std::chrono::system_clock::to_time_t(now);
    ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    std::cout << "[" << std::put_time(std::localtime(&serviceEndTime), "%Y-%m-%d %H:%M:%S.") << std::setw(3) << std::setfill('0') << ms.count() << "] " << client->getName() << " left the department " << name << std::endl;

    client->moveToNextDepartment();
    if (client->isDone()) {
        // Client pokidaet bank
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
        std::cout << "[" << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S.") << std::setw(3) << std::setfill('0') << ms.count() << "] Client " << client->getName() << " left the bank" << std::endl;
    } 
}

bool Department::isEmpty() const {
    std::unique_lock<std::mutex> lock(mtx);
    return clientQueue.empty();
}