#ifndef DEPARTMENT_H
#define DEPARTMENT_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>
#include <map>
#include <memory>
#include "Client.h"

class Department {
public:
    Department(const std::string& name, size_t numEmployees);

    void addClient(std::shared_ptr<Client> client);
    void workerAvailable();
    bool isEmpty() const;
    std::string getName() const { return name; }

private:
    std::string name;
    size_t numEmployees;
    std::queue<std::shared_ptr<Client>> clientQueue;
    mutable std::mutex mtx;
    std::condition_variable cv;
    std::map<std::shared_ptr<Client>, std::chrono::steady_clock::time_point> serviceStartTimes;
};

#endif // DEPARTMENT_H