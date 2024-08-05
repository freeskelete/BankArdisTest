#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <vector>
#include <memory>

class Client {
public:
    Client(const std::string& name, unsigned int serviceTime, unsigned int priority, const std::vector<std::string>& departments);

    const std::string& getName() const;
    unsigned int getPriority() const;
    const std::vector<std::string>& getDepartments() const;
    void moveToNextDepartment();
    bool isDone() const;

private:
    std::string name;
    unsigned int serviceTime;
    unsigned int priority;
    std::vector<std::string> departments;
    size_t currentDepartmentIndex;
};

#endif // CLIENT_H