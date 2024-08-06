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

    return 0;
}
