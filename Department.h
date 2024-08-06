#ifndef DEPARTMENT_H
#define DEPARTMENT_H

#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>
#include "Client.h"
#include "Logger.h"

class Department {
public:
    Department(const std::string& name, size_t numEmployees, std::shared_ptr<Logger> logger);
    void addClient(std::shared_ptr<Client> client);
    void workerAvailable();
    bool isEmpty() const;

private:
    std::string name;
    size_t numEmployees;
    std::queue<std::shared_ptr<Client>> clientQueue;
    mutable std::mutex mtx;
    std::condition_variable cv;
    std::shared_ptr<Logger> logger;
};

#endif // DEPARTMENT_H
