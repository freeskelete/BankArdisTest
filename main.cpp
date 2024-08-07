#include "Bank.h"
#include <iostream>

int main() {
    try {
        Bank bank("input.json");
        bank.open();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    std::cout << "[Input any button or press Enter to exit programm]" << std::endl;
    std::cin.get();

    return 0;
}
