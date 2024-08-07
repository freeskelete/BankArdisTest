#include "Bank.h"
#include <algorithm>
#include <thread>
#include "JsonLoader.h"

Bank::Bank(const std::string& jsonFile) : isBankOpen(false), logger(std::make_shared<Logger>()) {
    JsonLoader jsonLoader(jsonFile);
    jsonLoader.load();

    const auto& doc = jsonLoader.getDocument();

    const auto& departmentsArray = doc["departments"].GetArray();
    for (const auto& department : departmentsArray) {
        std::string name = department["name"].GetString();
        size_t employees = department["employees"].GetUint();
        departments[name] = std::make_shared<Department>(name, employees, logger);
    }

    const auto& clientsArray = doc["clients"].GetArray();
    for (const auto& client : clientsArray) {
        std::string name = client["name"].GetString();
        unsigned int serviceTime = client["time"].GetUint();
        unsigned int priority = client["priority"].GetUint();
        std::vector<std::string> departmentsList;
        for (const auto& dept : client["departments"].GetArray()) {
            departmentsList.push_back(dept.GetString());
        }
        clients.push_back(std::make_shared<Client>(name, serviceTime, priority, departmentsList));
    }
}

void Bank::open() {
    isBankOpen = true;
    logger->logBankOpened();

    std::vector<std::thread> clientThreads;
    for (auto& client : clients) {
        clientThreads.emplace_back(&Bank::processClient, this, client);
    }

    for (auto& thread : clientThreads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return allClientsServed(); });
    }

    close();
}

void Bank::processClient(const std::shared_ptr<Client>& client) {
    for (const auto& departmentName : client->getDepartments()) {
        auto department = departments[departmentName];
        department->addClient(client);
        department->workerAvailable();
    }

    std::unique_lock<std::mutex> lock(mtx);
    if (client->isDone()) {
        logger->logClientDone(client->getName());
    }
}

bool Bank::allClientsServed() const {
    return std::all_of(clients.begin(), clients.end(), [](const std::shared_ptr<Client>& client) {
        return client->isDone();
    });
}

void Bank::close() {
    isBankOpen = false;
    logger->logBankClosed();
}
