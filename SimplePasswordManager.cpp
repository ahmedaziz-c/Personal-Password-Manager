#include "SimplePasswordManager.h"
#include <iostream>
#include <conio.h>
#include <limits>

// Step 1: Constructor
SimplePasswordManager::SimplePasswordManager() : currentSelection(0) {
    updateDisplayItems();
}

// Step 2: Clear screen
void SimplePasswordManager::clearScreen() {
    system("cls");
}

// Step 3: Get input with prompt
std::string SimplePasswordManager::getInput(const std::string& prompt) {
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    return input;
}

// Step 4: Press any key to continue
void SimplePasswordManager::pressAnyKey() {
    std::cout << "\nPress any key to continue...";
    _getch();
}

// Step 5: Main application loop
void SimplePasswordManager::run() {
    while (true) {
        clearScreen();
        displayMenu();
        
        char choice = _getch();
        switch (choice) {
            case '1': displayAllEntries(); break;
            case '2': createNewEntry(); break;
            case '3': editEntry(); break;
            case '4': deleteEntry(); break;
            case '5': exportData(); break;
            case '6': importData(); break;
            case 'q':
            case 'Q':
                clearScreen();
                std::cout << "Goodbye! Stay secure!\n";
                return;
            default:
                std::cout << "\nInvalid choice! Press any key...";
                _getch();
        }
    }
}

// Step 6: Display main menu
void SimplePasswordManager::displayMenu() {
    std::cout << "========================================\n";
    std::cout << "        PERSONAL PASSWORD MANAGER\n";
    std::cout << "========================================\n";
    std::cout << "1. View All Passwords\n";
    std::cout << "2. Add New Password\n";
    std::cout << "3. Edit Password\n";
    std::cout << "4. Delete Password\n";
    std::cout << "5. Export to CSV\n";
    std::cout << "6. Import from CSV\n";
    std::cout << "Q. Quit\n";
    std::cout << "========================================\n";
    std::cout << "Choose an option (1-6 or Q): ";
}

// Step 7: Display all entries
void SimplePasswordManager::displayAllEntries() {
    clearScreen();
    std::cout << "========================================\n";
    std::cout << "            ALL PASSWORD ENTRIES\n";
    std::cout << "========================================\n\n";
    
    if (displayItems.empty()) {
        std::cout << "No password entries found.\n";
        pressAnyKey();
        return;
    }
    
    for (size_t i = 0; i < displayItems.size(); ++i) {
        if (i == currentSelection) {
            std::cout << "> " << displayItems[i] << " <\n";
        } else {
            std::cout << "  " << displayItems[i] << "\n";
        }
    }
    
    std::cout << "\n========================================\n";
    std::cout << "Controls: [Up/Down] Navigate  [E] Edit  [D] Delete  [C] Copy  [Enter] View Details  [Q] Back\n";
    std::cout << "Choice: ";
    
    char choice = _getch();
    switch (choice) {
        case 72: // Up arrow
            if (currentSelection > 0) currentSelection--;
            break;
        case 80: // Down arrow
            if (currentSelection < displayItems.size() - 1) currentSelection++;
            break;
        case 'e':
        case 'E':
            editEntry();
            break;
        case 'd':
        case 'D':
            deleteEntry();
            break;
        case 'c':
        case 'C':
            if (currentSelection < displayItems.size()) {
                auto parentIDs = manager.getAllParentIDs();
                if (currentSelection < parentIDs.size()) {
                    std::string selectedParent = parentIDs[currentSelection];
                    auto entries = manager.getEntriesByParent(selectedParent);
                    if (!entries.empty()) {
                        // Simple copy to clipboard alternative
                        std::cout << "\nPassword: " << entries[0].password << std::endl;
                        std::cout << "Password displayed above. Please copy manually.\n";
                        pressAnyKey();
                    }
                }
            }
            break;
        case '\r': // Enter key
        case '\n':
            if (currentSelection < displayItems.size()) {
                clearScreen();
                auto parentIDs = manager.getAllParentIDs();
                if (currentSelection < parentIDs.size()) {
                    std::string selectedParent = parentIDs[currentSelection];
                    auto entries = manager.getEntriesByParent(selectedParent);
                    if (!entries.empty()) {
                        const auto& entry = entries[0];
                        std::cout << "========================================\n";
                        std::cout << "           ENTRY DETAILS\n";
                        std::cout << "========================================\n";
                        std::cout << "Service:  " << entry.parentID << "\n";
                        std::cout << "Login:    " << entry.login << "\n";
                        std::cout << "Password: " << entry.password << "\n";
                        std::cout << "Notes:    " << entry.notes << "\n";
                        std::cout << "Created:  " << entry.dateCreated << "\n";
                        std::cout << "Updated:  " << entry.dateUpdated << "\n";
                        std::cout << "========================================\n";
                        pressAnyKey();
                    }
                }
            }
            break;
        case 'q':
        case 'Q':
            return;
    }
    
    updateDisplayItems();
}

// Step 8: Create new entry
void SimplePasswordManager::createNewEntry() {
    clearScreen();
    std::cout << "========================================\n";
    std::cout << "           CREATE NEW ENTRY\n";
    std::cout << "========================================\n\n";
    
    std::string parentID = getInput("Service/Website Name: ");
    
    if (manager.parentIDExists(parentID)) {
        std::cout << "\nERROR: '" << parentID << "' already exists!\n";
        pressAnyKey();
        return;
    }
    
    std::string login = getInput("Username/Login: ");
    std::string password = getInput("Password (press Enter to generate): ");
    
    if (password.empty()) {
        password = manager.generateRandomPassword();
        std::cout << "Generated Password: " << password << std::endl;
    }
    
    std::string notes = getInput("Notes (optional): ");
    
    if (manager.addEntry(parentID, login, password, notes)) {
        std::cout << "\nSUCCESS: Entry created!\n";
        updateDisplayItems();
    } else {
        std::cout << "\nERROR: Failed to create entry!\n";
    }
    
    pressAnyKey();
}

// Step 9: Edit entry
void SimplePasswordManager::editEntry() {
    if (displayItems.empty()) {
        std::cout << "No entries to edit.\n";
        pressAnyKey();
        return;
    }
    
    clearScreen();
    std::cout << "Select entry to edit (0 to cancel):\n\n";
    
    for (size_t i = 0; i < displayItems.size(); ++i) {
        std::cout << i + 1 << ". " << displayItems[i] << "\n";
    }
    
    std::cout << "\nChoice: ";
    std::string choiceStr = getInput("");
    
    try {
        int choice = std::stoi(choiceStr);
        if (choice == 0) return;
        
        size_t index = choice - 1;
        if (index < displayItems.size()) {
            auto parentIDs = manager.getAllParentIDs();
            if (index < parentIDs.size()) {
                std::string selectedParent = parentIDs[index];
                auto entries = manager.getEntriesByParent(selectedParent);
                
                if (!entries.empty()) {
                    auto& entry = entries[0];
                    
                    clearScreen();
                    std::cout << "Editing: " << entry.parentID << "\n\n";
                    
                    std::string login = getInput("New Login [" + entry.login + "]: ");
                    if (login.empty()) login = entry.login;
                    
                    std::string password = getInput("New Password [keep current]: ");
                    if (password.empty()) password = entry.password;
                    
                    std::string generate = getInput("Generate new password? (y/N): ");
                    if (generate == "y" || generate == "Y") {
                        password = manager.generateRandomPassword();
                        std::cout << "New Password: " << password << std::endl;
                    }
                    
                    std::string notes = getInput("New Notes [" + entry.notes + "]: ");
                    if (notes.empty()) notes = entry.notes;
                    
                    if (manager.updateEntry(entry.id, login, password, notes)) {
                        std::cout << "\nSUCCESS: Entry updated!\n";
                        updateDisplayItems();
                    } else {
                        std::cout << "\nERROR: Failed to update entry!\n";
                    }
                }
            }
        }
    } catch (...) {
        std::cout << "Invalid selection!\n";
    }
    
    pressAnyKey();
}

// Step 10: Delete entry
void SimplePasswordManager::deleteEntry() {
    if (displayItems.empty()) {
        std::cout << "No entries to delete.\n";
        pressAnyKey();
        return;
    }
    
    clearScreen();
    std::cout << "Select entry to delete (0 to cancel):\n\n";
    
    for (size_t i = 0; i < displayItems.size(); ++i) {
        std::cout << i + 1 << ". " << displayItems[i] << "\n";
    }
    
    std::cout << "\nChoice: ";
    std::string choiceStr = getInput("");
    
    try {
        int choice = std::stoi(choiceStr);
        if (choice == 0) return;
        
        size_t index = choice - 1;
        if (index < displayItems.size()) {
            auto parentIDs = manager.getAllParentIDs();
            if (index < parentIDs.size()) {
                std::string selectedParent = parentIDs[index];
                auto entries = manager.getEntriesByParent(selectedParent);
                
                if (!entries.empty()) {
                    std::string confirm = getInput("Are you sure you want to delete '" + selectedParent + "'? (y/N): ");
                    if (confirm == "y" || confirm == "Y") {
                        if (manager.deleteEntry(entries[0].id)) {
                            std::cout << "\nSUCCESS: Entry deleted!\n";
                            updateDisplayItems();
                        } else {
                            std::cout << "\nERROR: Failed to delete entry!\n";
                        }
                    }
                }
            }
        }
    } catch (...) {
        std::cout << "Invalid selection!\n";
    }
    
    pressAnyKey();
}

// Step 11: Export data
void SimplePasswordManager::exportData() {
    clearScreen();
    std::cout << "========================================\n";
    std::cout << "             EXPORT TO CSV\n";
    std::cout << "========================================\n\n";
    
    std::string filename = getInput("Enter filename (e.g., passwords.csv): ");
    
    if (manager.exportToCSV(filename)) {
        std::cout << "\nSUCCESS: Data exported to " << filename << "!\n";
    } else {
        std::cout << "\nERROR: Failed to export data!\n";
    }
    
    pressAnyKey();
}

// Step 12: Import data
void SimplePasswordManager::importData() {
    clearScreen();
    std::cout << "========================================\n";
    std::cout << "             IMPORT FROM CSV\n";
    std::cout << "========================================\n\n";
    
    std::string filename = getInput("Enter filename to import: ");
    
    if (manager.importFromCSV(filename)) {
        std::cout << "\nSUCCESS: Data imported from " << filename << "!\n";
        updateDisplayItems();
    } else {
        std::cout << "\nERROR: Failed to import data!\n";
    }
    
    pressAnyKey();
}

// Step 13: Update display items
void SimplePasswordManager::updateDisplayItems() {
    displayItems.clear();
    auto parentIDs = manager.getAllParentIDs();
    
    for (const auto& parent : parentIDs) {
        auto entries = manager.getEntriesByParent(parent);
        if (!entries.empty()) {
            displayItems.push_back(parent + " (" + entries[0].login + ")");
        }
    }
}