#ifndef SIMPLE_PASSWORD_MANAGER_H
#define SIMPLE_PASSWORD_MANAGER_H

#include "PasswordManager.h"
#include <string>
#include <vector>

class SimplePasswordManager {
private:
    PasswordManager manager;
    size_t currentSelection;
    std::vector<std::string> displayItems;

public:
    SimplePasswordManager();
    void run();

private:
    void clearScreen();
    void displayMenu();
    void displayAllEntries();
    void createNewEntry();
    void editEntry();
    void deleteEntry();
    void exportData();
    void importData();
    std::string getInput(const std::string& prompt);
    void updateDisplayItems();
    void pressAnyKey();
};

#endif
