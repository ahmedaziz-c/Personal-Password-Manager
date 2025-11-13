// Step 1: Main application file - SIMPLIFIED VERSION
#include "SimplePasswordManager.h"
#include <iostream>

int main() {
    try {
        std::cout << "Starting Password Manager...\n";
        SimplePasswordManager app;
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cout << "Press any key to exit...";
        std::cin.get();
        return 1;
    }
    
    return 0;
}