#ifndef BANK_H
#define BANK_H

#include <unordered_map>
#include <vector>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <queue>
#include "Department.h"
#include "Client.h"
#include "Logger.h"

class Bank {
private:
    std::unordered_map<std::string, std::shared_ptr<Department>> departments;
    std::vector<std::shared_ptr<Client>> clients;
    std::shared_ptr<Logger> logger;
    bool isBankOpen;
    std::mutex mtx;
    std::condition_variable cv;

    void processClient(const std::shared_ptr<Client>& client);
    bool allClientsServed() const;

public:
    explicit Bank(const std::string& jsonFile);
    void open();
    void close();
};

#endif // BANK_H
