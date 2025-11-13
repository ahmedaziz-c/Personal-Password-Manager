#ifndef PASSWORD_MANAGER_TUI_H
#define PASSWORD_MANAGER_TUI_H

#include "PasswordManager.h"

// Windows-compatible TUI implementation
class PasswordManagerTUI {
private:
    PasswordManager manager;
    
    // UI State
    int currentSelection;
    bool leftPanelActive;
    std::vector<std::string> displayItems;
    std::string statusMessage;
    
public:
    PasswordManagerTUI();
    
    // Step 3.2: Main application loop
    void run();
    
private:
    // Step 3.3: UI methods
    void clearScreen();
    void displayUI();
    
    // Step 3.4: Panel rendering
    void renderLeftPanel();
    void renderRightPanel();
    void renderStatusBar();
    
    // Step 3.5: Input handling
    void handleInput();
    void switchPanels();
    void createNewEntry();
    void editCurrentEntry();
    void copyToClipboard(const std::string& text);
    
    // Step 3.6: Utility methods
    void updateDisplayItems();
    void showMessage(const std::string& message);
    std::string getInput(const std::string& prompt);
};

#endif
