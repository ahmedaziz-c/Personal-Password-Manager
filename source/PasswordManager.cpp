#include "PasswordManager.h"

// Step 2.1: Add entry with duplication prevention
bool PasswordManager::addEntry(const std::string& parentID, const std::string& login, 
                              const std::string& password, const std::string& notes) {
    // Step 2.2: Check if parentID already exists
    if (parentIDExists(parentID)) {
        return false; // Duplicate detected
    }
    
    // Step 2.3: Create and add new entry
    PasswordEntry newEntry(parentID, login, password, notes);
    newEntry.id = nextID++;
    entries.push_back(newEntry);
    parentIDs.insert(parentID);
    
    return true;
}

// Step 2.4: Update existing entry
bool PasswordManager::updateEntry(int id, const std::string& login, 
                                 const std::string& password, const std::string& notes) {
    for (auto& entry : entries) {
        if (entry.id == id) {
            entry.login = login;
            entry.password = password;
            entry.notes = notes;
            
            // Update timestamp
            time_t now = time(0);
            entry.dateUpdated = ctime(&now);
            entry.dateUpdated.erase(entry.dateUpdated.find_last_not_of("\n") + 1);
            
            return true;
        }
    }
    return false;
}

// Step 2.5: Delete entry
bool PasswordManager::deleteEntry(int id) {
    for (auto it = entries.begin(); it != entries.end(); ++it) {
        if (it->id == id) {
            parentIDs.erase(it->parentID);
            entries.erase(it);
            return true;
        }
    }
    return false;
}

// Step 2.6: Check for duplicate parent IDs
bool PasswordManager::parentIDExists(const std::string& parentID) const {
    return parentIDs.find(parentID) != parentIDs.end();
}

// Step 2.7: Generate random password
std::string PasswordManager::generateRandomPassword(int length) const {
    const std::string chars = 
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "!@#$%^&*";
    
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution(0, chars.size() - 1);
    
    std::string password;
    for (int i = 0; i < length; ++i) {
        password += chars[distribution(generator)];
    }
    
    return password;
}

// Step 2.8: CSV Export functionality
bool PasswordManager::exportToCSV(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) return false;
    
    // Write header
    file << "ID,ParentID,Login,Password,Notes,DateCreated,DateUpdated\n";
    
    // Write data
    for (const auto& entry : entries) {
        file << entry.id << ","
             << "\"" << entry.parentID << "\","
             << "\"" << entry.login << "\","
             << "\"" << entry.password << "\","
             << "\"" << entry.notes << "\","
             << "\"" << entry.dateCreated << "\","
             << "\"" << entry.dateUpdated << "\"\n";
    }
    
    file.close();
    return true;
}

// Step 2.9: CSV Import functionality  
bool PasswordManager::importFromCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;
    
    std::string line;
    std::getline(file, line); // Skip header
    
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;
        
        while (std::getline(ss, token, ',')) {
            // Remove quotes
            if (!token.empty() && token.front() == '"' && token.back() == '"') {
                token = token.substr(1, token.size() - 2);
            }
            tokens.push_back(token);
        }
        
        if (tokens.size() >= 6) {
            // Only add if not duplicate
            if (!parentIDExists(tokens[1])) {
                PasswordEntry newEntry(tokens[1], tokens[2], tokens[3], tokens[4]);
                newEntry.id = nextID++;
                newEntry.dateCreated = tokens[5];
                newEntry.dateUpdated = tokens.size() > 6 ? tokens[6] : tokens[5];
                entries.push_back(newEntry);
                parentIDs.insert(tokens[1]);
            }
        }
    }
    
    file.close();
    return true;
}

// Step 2.10: Utility methods
std::vector<PasswordEntry> PasswordManager::getEntriesByParent(const std::string& parentID) const {
    std::vector<PasswordEntry> result;
    for (const auto& entry : entries) {
        if (entry.parentID == parentID) {
            result.push_back(entry);
        }
    }
    return result;
}

std::vector<std::string> PasswordManager::getAllParentIDs() const {
    return std::vector<std::string>(parentIDs.begin(), parentIDs.end());
}

PasswordEntry* PasswordManager::getEntry(int id) {
    for (auto& entry : entries) {
        if (entry.id == id) {
            return &entry;
        }
    }
    return nullptr;
}
