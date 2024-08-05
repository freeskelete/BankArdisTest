#include "Client.h"

Client::Client(const std::string& name, unsigned int serviceTime, unsigned int priority, const std::vector<std::string>& departments)
    : name(name), serviceTime(serviceTime), priority(priority), departments(departments), currentDepartmentIndex(0) {}

const std::string& Client::getName() const { return name; }
unsigned int Client::getPriority() const { return priority; }
const std::vector<std::string>& Client::getDepartments() const { return departments; }

void Client::moveToNextDepartment() {
    if (currentDepartmentIndex <= departments.size()) {
        ++currentDepartmentIndex;
    }
}

bool Client::isDone() const {
    return currentDepartmentIndex >= departments.size();
}
