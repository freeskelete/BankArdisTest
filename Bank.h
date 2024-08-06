#ifndef BANK_H
#define BANK_H

#include <unordered_map>
#include <vector>
#include <memory>
#include <mutex>
#include <condition_variable>
#include "Department.h"
#include "Client.h"
#include "Logger.h"

class Bank {
private:
    void processClients();
    void handleClientInDepartment(const std::shared_ptr<Client>& client, const std::shared_ptr<Department>& department);
    bool allClientsServed() const;

    std::unordered_map<std::string, std::shared_ptr<Department>> departments;
    std::vector<std::shared_ptr<Client>> clients;
    std::shared_ptr<Logger> logger;
    bool isBankOpen;
    std::mutex mtx;
    std::condition_variable cv;   
public:
    explicit Bank(const std::string& jsonFile);
    void open();
    void close();
};

#endif // BANK_H
