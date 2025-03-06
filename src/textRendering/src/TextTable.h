#pragma once

#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class TextTable {
public:
    // Constructor
    TextTable(const std::string& filepath);

    // Load the tab-delimited file
    void LoadCSV(const std::string& filePath);

    // Set the current language
    void SetLanguage(const std::string& language);

    // Retrieve a string by ID for the current language
    std::string GetString(const std::string& id) const;

private:
    // Data structure to hold all strings
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> m_data;

    // Currently selected language
    std::string m_currentLanguage;
};
