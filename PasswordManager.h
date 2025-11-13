#ifndef PASSWORD_MANAGER_H
#define PASSWORD_MANAGER_H

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <ctime>
#include <fstream>
#include <sstream>
#include <random>
#include <functional>

// Step 1.1: Define the data structure for password entries
struct PasswordEntry {
    int id;
    std::string parentID;        // Entity name (Amazon, Facebook, etc.)
    std::string login;
    std::string password;
    std::string notes;
    std::string dateCreated;
    std::string dateUpdated;
    
    // Step 1.2: Constructor for new entries
    PasswordEntry(const std::string& parent, const std::string& user, 
                  const std::string& pwd, const std::string& note = "")
        : parentID(parent), login(user), password(pwd), notes(note) {
        // Step 1.3: Generate timestamps
        time_t now = time(0);
        dateCreated = ctime(&now);
        dateUpdated = dateCreated;
        // Remove newline from timestamp
        dateCreated.erase(dateCreated.find_last_not_of("\n") + 1);
        dateUpdated = dateCreated;
    }
};

// Step 1.4: Main Password Manager Class
class PasswordManager {
private:
    std::vector<PasswordEntry> entries;
    std::set<std::string> parentIDs; // For duplication prevention
    int nextID;
    
public:
    PasswordManager() : nextID(1) {}
    
    // Step 1.5: Core functionality methods
    bool addEntry(const std::string& parentID, const std::string& login, 
                  const std::string& password, const std::string& notes = "");
    bool updateEntry(int id, const std::string& login, const std::string& password, 
                     const std::string& notes = "");
    bool deleteEntry(int id);
    std::vector<PasswordEntry> getEntriesByParent(const std::string& parentID) const;
    std::vector<std::string> getAllParentIDs() const;
    PasswordEntry* getEntry(int id);
    
    // Step 1.6: Import/Export functionality
    bool exportToCSV(const std::string& filename) const;
    bool importFromCSV(const std::string& filename);
    
    // Step 1.7: Password generation
    std::string generateRandomPassword(int length = 16) const;
    
    // Step 1.8: Duplication prevention
    bool parentIDExists(const std::string& parentID) const;
    
    // Step 1.9: Get all entries
    const std::vector<PasswordEntry>& getAllEntries() const { return entries; }
};

#endif