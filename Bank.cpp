#include "Bank.h"
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <exception>
#include "JsonLoader.h"

Bank::Bank(const std::string& jsonFile) : isBankOpen(false) {
    JsonLoader jsonLoader(jsonFile);
    jsonLoader.load();

    const auto& doc = jsonLoader.getDocument();

    const auto& departmentsArray = doc["departments"].GetArray();
    for (const auto& department : departmentsArray) {
        std::string name = department["name"].GetString();
        size_t employees = department["employees"].GetUint();
        departments[name] = std::make_shared<Department>(name, employees);
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

    logger = std::make_shared<Logger>();
}

void Bank::open() {
    logger->start();
    isBankOpen = true;

    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    std::cout << "[" << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S.") << std::setw(3) << std::setfill('0') << ms.count() << "] Bank opened" << std::endl;

    std::thread processingThread(&Bank::processClients, this);

    // Wait until all clients are served
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return allClientsServed(); });
    }

    // Close the bank
    close();

    if (processingThread.joinable()) {
        processingThread.join();
    }
}

void Bank::processClients() {

    std::sort(clients.begin(), clients.end(), [](const std::shared_ptr<Client>& a, const std::shared_ptr<Client>& b) {
        return a->getPriority() < b->getPriority();
    });

    for (auto& client : clients) {
        if (!client->isDone()) {
            const auto& departmentName = client->getDepartments()[0];
            if (departments.find(departmentName) != departments.end()) {
                handleClientInDepartment(client, departments[departmentName]);
            }
        }
    }

    updateDepartments();
}

void Bank::handleClientInDepartment(const std::shared_ptr<Client>& client, const std::shared_ptr<Department>& department) {
    department->addClient(client);

    std::thread([this, client, department]() {
        department->workerAvailable();
        if (!client->isDone()) {
            const auto& nextDepartmentName = client->getDepartments()[client->getDepartments().size() - 1];
            if (departments.find(nextDepartmentName) != departments.end()) {
                handleClientInDepartment(client, departments[nextDepartmentName]);
            }
        }

        std::unique_lock<std::mutex> lock(mtx);
        if (allClientsServed()) {
            cv.notify_all();
        }
    }).detach();
}

void Bank::updateDepartments() {
    while (isBankOpen && !allClientsServed()) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return !isBankOpen || allClientsServed(); });

        for (auto& [name, department] : departments) {
            department->workerAvailable();
        }
    }
}

bool Bank::allClientsServed() const {
    return std::all_of(clients.begin(), clients.end(), [](const std::shared_ptr<Client>& client) {
        return client->isDone();
    });
}

void Bank::close() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    std::cout << "[" << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S.") << std::setw(3) << std::setfill('0') << ms.count() << "] Bank closed" << std::endl;
}
