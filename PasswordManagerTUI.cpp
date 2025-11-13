#include "PasswordManagerTUI.h"
#include <iostream>
#include <conio.h>  // Windows console input
#include <windows.h> // Windows API for clipboard

// Step 4.1: Constructor - FIXED: changed currentSelection to size_t
PasswordManagerTUI::PasswordManagerTUI() 
    : currentSelection(0), leftPanelActive(true), 
      statusMessage("Welcome to Password Manager - F2:New F3:Edit F4:GenPwd Tab:Switch") {}

// Step 4.2: Clear screen (Windows compatible)
void PasswordManagerTUI::clearScreen() {
    system("cls");
}

// Step 4.3: Get input with prompt
std::string PasswordManagerTUI::getInput(const std::string& prompt) {
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    return input;
}

// Step 4.4: Main application loop
void PasswordManagerTUI::run() {
    updateDisplayItems();
    
    while (true) {
        displayUI();
        handleInput();
    }
}

// Step 4.5: Display complete UI
void PasswordManagerTUI::displayUI() {
    clearScreen();
    
    // Draw main header
    std::cout << "=============================================================\n";
    std::cout << "               PERSONAL PASSWORD MANAGER\n";
    std::cout << "=============================================================\n\n";
    
    // Draw panels
    std::cout << "+-----------------------------+-----------------------------+\n";
    std::cout << "|        ENTRIES LIST         |        ENTRY DETAILS        |\n";
    std::cout << "+-----------------------------+-----------------------------+\n";
    
    // Calculate positions for dual panels
    int panelHeight = 15;
    
    // Render both panels side by side - FIXED: Now these functions are declared
    for (int i = 0; i < panelHeight; i++) {
        std::cout << "| ";
        renderLeftPanelLine(i);
        std::cout << " | ";
        renderRightPanelLine(i);
        std::cout << " |\n";
    }
    
    std::cout << "+-----------------------------+-----------------------------+\n";
    
    // Status bar
    renderStatusBar();
    
    // Instructions
    std::cout << "\nCONTROLS: [↑↓] Navigate  [TAB] Switch Panels  [F2] New Entry";
    std::cout << "\n          [F3] Edit  [F4] Generate Password  [C] Copy  [Q] Quit\n";
}

// Step 4.6: Render left panel line - FIXED: Added proper implementation
void PasswordManagerTUI::renderLeftPanelLine(int line) {
    const int panelWidth = 27;
    std::string content = "";
    
    if (line == 0) {
        content = "=== SERVICES ===";
    } else if (line - 2 < static_cast<int>(displayItems.size()) && line >= 2) {
        int index = line - 2;
        if (index == static_cast<int>(currentSelection) && leftPanelActive) {
            content = "> " + displayItems[index];
        } else {
            content = "  " + displayItems[index];
        }
    }
    
    // Pad and truncate to fit panel
    if (content.length() > panelWidth) {
        content = content.substr(0, panelWidth);
    } else {
        content.append(panelWidth - content.length(), ' ');
    }
    std::cout << content;
}

// Step 4.7: Render right panel line - FIXED: Added proper implementation
void PasswordManagerTUI::renderRightPanelLine(int line) {
    const int panelWidth = 27;
    std::string content = "";
    
    if (currentSelection < displayItems.size()) {
        auto parentIDs = manager.getAllParentIDs();
        if (currentSelection < parentIDs.size()) {
            std::string selectedParent = parentIDs[currentSelection];
            auto entries = manager.getEntriesByParent(selectedParent);
            
            if (!entries.empty()) {
                const auto& entry = entries[0];
                
                switch (line) {
                    case 1: content = "Service: " + entry.parentID; break;
                    case 2: content = "Login: " + entry.login; break;
                    case 3: content = "Password: ********"; break;
                    case 4: content = "Notes: " + entry.notes; break;
                    case 6: content = "Created: " + entry.dateCreated; break;
                    case 7: content = "Updated: " + entry.dateUpdated; break;
                    case 9: content = "Press 'C' to copy"; break;
                    case 10: content = "password to clipboard"; break;
                }
            }
        }
    }
    
    if (content.empty() && line == 1) {
        content = "Select an entry to view";
    }
    
    // Pad and truncate to fit panel
    if (content.length() > panelWidth) {
        content = content.substr(0, panelWidth);
    } else {
        content.append(panelWidth - content.length(), ' ');
    }
    std::cout << content;
}

// Step 4.8: Render status bar
void PasswordManagerTUI::renderStatusBar() {
    std::cout << "\nSTATUS: " << statusMessage;
}

// Step 4.9: Handle keyboard input - FIXED: size_t comparisons
void PasswordManagerTUI::handleInput() {
    if (_kbhit()) {
        int ch = _getch();
        
        // Check for function keys (F1-F4)
        if (ch == 0 || ch == 224) {
            int ext = _getch();
            switch (ext) {
                case 59: // F1 - Help
                    showMessage("Help: Use arrow keys to navigate, TAB to switch panels");
                    break;
                case 60: // F2 - New Entry
                    createNewEntry();
                    break;
                case 61: // F3 - Edit
                    editCurrentEntry();
                    break;
                case 62: // F4 - Generate Password
                    showMessage("Generated: " + manager.generateRandomPassword());
                    break;
            }
        } else {
            switch (ch) {
                case 'q':
                case 'Q':
                    clearScreen();
                    std::cout << "Goodbye!\n";
                    exit(0);
                case '\t': // TAB key
                    switchPanels();
                    break;
                case 'c':
                case 'C':
                    if (currentSelection < manager.getAllParentIDs().size()) {
                        auto parentIDs = manager.getAllParentIDs();
                        std::string selectedParent = parentIDs[currentSelection];
                        auto entries = manager.getEntriesByParent(selectedParent);
                        if (!entries.empty()) {
                            copyToClipboard(entries[0].password);
                        }
                    }
                    break;
                case 72: // Up arrow
                    if (currentSelection > 0) currentSelection--;
                    break;
                case 80: // Down arrow
                    if (currentSelection < displayItems.size() - 1) currentSelection++;
                    break;
            }
        }
        
        updateDisplayItems();
    }
}

// Step 4.10: Switch between panels
void PasswordManagerTUI::switchPanels() {
    leftPanelActive = !leftPanelActive;
    statusMessage = leftPanelActive ? 
        "Left Panel Active - Navigate with Up/Down" : 
        "Right Panel Active - Press 'C' to copy password";
}

// Step 4.11: Create new entry
void PasswordManagerTUI::createNewEntry() {
    clearScreen();
    std::cout << "=== CREATE NEW ENTRY ===\n\n";
    
    std::string parentID = getInput("Service Name (e.g., Amazon, Facebook): ");
    
    // Check for duplicates
    if (manager.parentIDExists(parentID)) {
        std::cout << "ERROR: Service '" << parentID << "' already exists!\n";
        std::cout << "Press Enter to continue...";
        std::cin.ignore();
        return;
    }
    
    std::string login = getInput("Login/Username: ");
    std::string password = getInput("Password (or press Enter to generate): ");
    
    if (password.empty()) {
        password = manager.generateRandomPassword();
        std::cout << "Generated Password: " << password << std::endl;
    }
    
    std::string notes = getInput("Notes (optional): ");
    
    if (manager.addEntry(parentID, login, password, notes)) {
        showMessage("Entry created successfully!");
    } else {
        showMessage("Failed to create entry (duplicate detected)");
    }
    
    updateDisplayItems();
}

// Step 4.12: Edit current entry - FIXED: size_t comparison
void PasswordManagerTUI::editCurrentEntry() {
    if (currentSelection >= manager.getAllParentIDs().size()) return;
    
    clearScreen();
    
    auto parentIDs = manager.getAllParentIDs();
    std::string selectedParent = parentIDs[currentSelection];
    auto entries = manager.getEntriesByParent(selectedParent);
    
    if (entries.empty()) return;
    
    auto& entry = entries[0];
    
    std::cout << "=== EDIT ENTRY: " << entry.parentID << " ===\n\n";
    
    std::string login = getInput("Login [" + entry.login + "]: ");
    if (login.empty()) login = entry.login;
    
    std::string password = getInput("Password [********]: ");
    if (password.empty()) password = entry.password;
    
    std::string generate = getInput("Generate new password? (y/N): ");
    if (generate == "y" || generate == "Y") {
        password = manager.generateRandomPassword();
        std::cout << "New Password: " << password << std::endl;
    }
    
    std::string notes = getInput("Notes [" + entry.notes + "]: ");
    if (notes.empty()) notes = entry.notes;
    
    if (manager.updateEntry(entry.id, login, password, notes)) {
        showMessage("Entry updated successfully!");
    } else {
        showMessage("Failed to update entry");
    }
    
    updateDisplayItems();
}

// Step 4.13: Copy to clipboard (Windows)
void PasswordManagerTUI::copyToClipboard(const std::string& text) {
    if (OpenClipboard(nullptr)) {
        EmptyClipboard();
        HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1);
        if (hGlobal) {
            memcpy(GlobalLock(hGlobal), text.c_str(), text.size() + 1);
            GlobalUnlock(hGlobal);
            SetClipboardData(CF_TEXT, hGlobal);
        }
        CloseClipboard();
        showMessage("Password copied to clipboard!");
    } else {
        showMessage("Failed to copy to clipboard");
    }
}

// Step 4.14: Utility methods
void PasswordManagerTUI::updateDisplayItems() {
    displayItems.clear();
    auto parentIDs = manager.getAllParentIDs();
    
    for (const auto& parent : parentIDs) {
        auto entries = manager.getEntriesByParent(parent);
        if (!entries.empty()) {
            // Truncate long names for display
            std::string displayName = parent;
            if (displayName.length() > 20) {
                displayName = displayName.substr(0, 17) + "...";
            }
            displayItems.push_back(displayName + " (" + entries[0].login + ")");
        }
    }
}

void PasswordManagerTUI::showMessage(const std::string& message) {
    statusMessage = message;
}
