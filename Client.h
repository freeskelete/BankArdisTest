#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <vector>

class Client {
private:
    std::string name;
    unsigned int serviceTime;
    unsigned int priority;
    std::vector<std::string> departments;
    size_t currentDepartmentIndex;    
public:
    Client(const std::string& name, unsigned int serviceTime, unsigned int priority, const std::vector<std::string>& departments);

    std::string getName() const;
    unsigned int getServiceTime() const;
    unsigned int getPriority() const;
    const std::vector<std::string>& getDepartments() const;
    size_t getCurrentDepartmentIndex() const;
    bool isDone() const;
    void completeCurrentTask();
};

#endif // CLIENT_H
