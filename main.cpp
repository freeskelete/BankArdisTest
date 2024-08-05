#include "Bank.h"
#include <iostream>

int main() {
    try {
        Bank bank("input.json");
        bank.open();

        // Simulate some operations
        std::this_thread::sleep_for(std::chrono::seconds(10));

        bank.close();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
