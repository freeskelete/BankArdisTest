#include "Client.h"

Client::Client(const std::string& name, unsigned int serviceTime, unsigned int priority, const std::vector<std::string>& departments)
    : name(name), serviceTime(serviceTime), priority(priority), departments(departments), currentDepartmentIndex(0) {}

std::string Client::getName() const {
    return name;
}

unsigned int Client::getServiceTime() const {
    return serviceTime;
}

unsigned int Client::getPriority() const {
    return priority;
}

const std::vector<std::string>& Client::getDepartments() const {
    return departments;
}

void Client::moveToNextDepartment() {
    if (currentDepartmentIndex < departments.size() - 1) {
        ++currentDepartmentIndex;
    }
}

bool Client::isDone() const {
    return currentDepartmentIndex >= departments.size() - 1;
}
