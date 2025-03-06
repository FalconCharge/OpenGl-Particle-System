#include "TextTable.h"
// Default constructor
TextTable::TextTable(const std::string& fileName){
    SetLanguage("English");
    LoadCSV(fileName);
};
//Loads the information from a tsv file and stores it in the class
void TextTable::LoadCSV(const std::string& filePath){
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return;
    }
    
    std::string line;
    std::getline(file, line);   //The header line

    std::stringstream ss(line);
    std::string lang;
    std::vector<std::string> languages;

    // Skip the first attribute (ID label)
    std::getline(ss, lang, '\t');

    //Put all the languages in a single vector
    while(std::getline(ss, lang, '\t')){
        languages.push_back(lang);
    }

    while(std::getline(file, line)){
        std::stringstream ssLine(line);
        std::string id;
        std::getline(ssLine, id, '\t');  // Read the ID

        for(size_t i = 0; i < languages.size(); i++){
            std::string translation;
            if (i < languages.size() - 1) {
                std::getline(ssLine, translation, '\t');
            } else {
                std::getline(ssLine, translation);  // No tab after the last translation
            }
            m_data[id][languages[i]] = translation;
        }
    }
}
// Returns the String connected to the id dependant on the current language
std::string TextTable::GetString(const std::string& id) const {
    auto langIt = m_data.find(id);
    if (langIt != m_data.end()) {
        auto translationIt = langIt->second.find(m_currentLanguage);
        if (translationIt != langIt->second.end()) {
            return translationIt->second;
        }
    }

    // Return a default message if the ID or language is not found
    return "String not found";
}
// Sets the current language of where to get the values
void TextTable::SetLanguage(const std::string& language){
    m_currentLanguage = language;
}