// Step 5.1: Main application file
#include "PasswordManagerTUI.h"
#include <iostream>

int main() {
    try {
        // Step 5.2: Create and run TUI application
        PasswordManagerTUI app;
        app.run();
    } catch (const std::exception& e) {
        // Step 5.3: Error handling
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
