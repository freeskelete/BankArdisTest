#include "Department.h"
#include <chrono>
#include <thread>

Department::Department(const std::string& name, size_t numEmployees, std::shared_ptr<Logger> logger)
    : name(name), numEmployees(numEmployees), logger(logger) {}

void Department::addClient(std::shared_ptr<Client> client) {
    {
        std::unique_lock<std::mutex> lock(mtx);
        clientQueue.push(client);
    }
    cv.notify_one();
    logger->logClientArrived(client->getName(), name);
}

void Department::workerAvailable() {
    std::unique_lock<std::mutex> lock(mtx);
    while (clientQueue.empty()) {
        cv.wait(lock);
    }

    auto client = clientQueue.front();
    clientQueue.pop();

    lock.unlock();
    logger->logClientServed(client->getName(), name);

    std::this_thread::sleep_for(std::chrono::milliseconds(client->getServiceTime()));

    client->completeCurrentTask();
    logger->logClientLeft(client->getName(), name);

}

bool Department::isEmpty() const {
    std::unique_lock<std::mutex> lock(mtx);
    return clientQueue.empty();
}
