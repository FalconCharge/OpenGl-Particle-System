#include "ProcessFile.h"

//Constructor
ProcessFiles::ProcessFiles(const std::string fileName) : m_font() {
    ProcessFile(fileName);
}
// Starts processing the file
void ProcessFiles::ProcessFile(std::string fileName){

    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "Error opening descriptor file: " << fileName << std::endl;
        return;
    }
    std::string line;   //stores the current line
    while(std::getline(file, line)){
        ProcessLine(line);
    }
}
// Checks for a certain line and if found start operating on it
void ProcessFiles::ProcessLine(std::string line){
    line = TrimLine(line);
    
    // Process lines within the <chars> section
    if (line.rfind("<char ", 0) == 0) {
        parseCharacterLine(line);
    }else if(line.rfind("<info ", 0) == 0){
        parseInfoLine(line);
    }else if(line.rfind("<common ", 0) == 0){
        parseCommonLine(line);
    }else if(line.rfind("<page ", 0) == 0){
        parsePageLine(line);
    }else if(line.rfind("<kerning ", 0)== 0){
        parseKerningLine(line);
    }
}
// Parse the character line
void ProcessFiles::parseCharacterLine(const std::string& line){
    Character ch;

    std::stringstream ss(line);
    std::string attribute;

    while(std::getline(ss, attribute, ' ')){
        size_t pos = attribute.find('=');
        if(pos != std::string::npos){
            std::string key = attribute.substr(0, pos);
            std::string value = attribute.substr(pos+1);
            //CleanUP
            value.erase(std::remove(value.begin(), value.end(), '"'), value.end());
            value.erase(std::remove(value.begin(), value.end(), '/'), value.end());
            //Assign the values to the Character class
            if (key == "id") ch.id = std::stoi(value);
            else if (key == "x") ch.x = std::stoi(value);
            else if (key == "y") ch.y = std::stoi(value);
            else if (key == "width") ch.width = std::stoi(value);
            else if (key == "height") ch.height = std::stoi(value);
            else if (key == "xoffset") ch.xOffset = std::stoi(value);
            else if (key == "yoffset") ch.yOffset = std::stoi(value);
            else if (key == "xadvance") ch.xAdvance = std::stoi(value);
            else if (key == "page") ch.page = std::stoi(value);
            else if (key == "chnl") ch.chnl = std::stoi(value);
        }
    }
    m_font.AddCharacter(ch);
}
// Parase the common line
void ProcessFiles::parseCommonLine(const std::string& line){
    Common common;

    std::stringstream ss(line);
    std::string attribute;

    while(std::getline(ss, attribute, ' ')){
        size_t pos = attribute.find('=');
        if(pos != std::string::npos){
            std::string key = attribute.substr(0, pos);
            std::string value = attribute.substr(pos+1);
            //CleanUP
            value.erase(std::remove(value.begin(), value.end(), '"'), value.end());
            value.erase(std::remove(value.begin(), value.end(), '/'), value.end());
            if (key == "lineHeight") common.lineHeight = std::stoi(value);
            else if (key == "base") common.base = std::stoi(value);
            else if (key == "scaleW") common.scaleW = std::stoi(value);
            else if (key == "scaleH") common.scaleH = std::stoi(value);
            else if (key == "pages") common.pages = std::stoi(value);
            else if (key == "packed") common.packed = std::stoi(value);
            else if (key == "alphaChnl") common.alphaChnl = std::stoi(value);
            else if (key == "redChnl") common.redChnl = std::stoi(value);
            else if (key == "greenChnl") common.greenChnl = std::stoi(value);
            else if (key == "blueChnl") common.blueChnl = std::stoi(value);
        }
    }
    m_font.AddCommon(common);
}
// Parse the Info line
void ProcessFiles::parseInfoLine(const std::string& line) {
    
    Info info;

    std::stringstream ss(line);
    std::string attribute;

    while (std::getline(ss, attribute, ' ')) {
        size_t pos = attribute.find('=');
        if (pos != std::string::npos) {
            std::string key = attribute.substr(0, pos);
            std::string value = attribute.substr(pos + 1);
            // Clean up the value
            value.erase(std::remove(value.begin(), value.end(), '"'), value.end());
            value.erase(std::remove(value.begin(), value.end(), '/'), value.end());

            // Assign values to the Info object
            if (key == "face") info.face = value;  // Assign directly for strings
            else if (key == "size") info.size = std::stoi(value);
            else if (key == "bold") info.bold = std::stoi(value) != 0;  // Convert to bool
            else if (key == "italic") info.italic = std::stoi(value) != 0;  // Convert to bool
            else if (key == "charset") info.charset = value;  // Assign directly for strings
            else if (key == "unicode") info.unicode = std::stoi(value) != 0;  // Convert to bool
            else if (key == "stretchH") info.stretchH = std::stoi(value);
            else if (key == "smooth") info.smooth = std::stoi(value) != 0;  // Convert to bool
            else if (key == "aa") info.aa = std::stoi(value);
            else if (key == "outline") info.outline = std::stoi(value);

            // Parse padding as an array of 4 integers
            else if (key == "padding") {
                std::stringstream paddingStream(value);
                std::string paddingValue;
                int i = 0;
                while (std::getline(paddingStream, paddingValue, ',')) {
                    if (i < 4) {
                        info.padding[i] = std::stoi(paddingValue);
                    }
                    i++;
                }
            }

            // Parse spacing as an array of 2 integers
            else if (key == "spacing") {
                std::stringstream spacingStream(value);
                std::string spacingValue;
                int i = 0;
                while (std::getline(spacingStream, spacingValue, ',')) {
                    if (i < 2) {
                        info.spacing[i] = std::stoi(spacingValue);
                    }
                    i++;
                }
            }
        }
    }
    // Add the populated Info object to m_font
    m_font.AddInfo(info);
}
//Parse the page line
void ProcessFiles::parsePageLine(const std::string& line){

    std::stringstream ss(line);
    std::string attribute;
    int id;
    std::string file;

    while(std::getline(ss, attribute, ' ')){
        size_t pos = attribute.find('=');
        if(pos != std::string::npos){
            std::string key = attribute.substr(0, pos);
            std::string value = attribute.substr(pos+1);
            //CleanUP
            value.erase(std::remove(value.begin(), value.end(), '"'), value.end());
            value.erase(std::remove(value.begin(), value.end(), '/'), value.end());
            if (key == "id") id = std::stoi(value);
            else if (key == "file") file = value;
        }   
    }
    Page page(id, file);
    m_font.AddPage(page);
}
// Parse the kerning line
void ProcessFiles::parseKerningLine(const std::string& line){


    std::stringstream ss(line);
    std::string attribute;
    int first = 0, second = 0, amount = 0;
    while(std::getline(ss, attribute, ' ')){
        size_t pos = attribute.find('=');
        if(pos != std::string::npos){
            std::string key = attribute.substr(0, pos);
            std::string value = attribute.substr(pos+1);

            value.erase(std::remove(value.begin(), value.end(), '"'), value.end());
            value.erase(std::remove(value.begin(), value.end(), '/'), value.end());

            if (key == "first")  first = std::stoi(value);
            else if (key == "second")  second = std::stoi(value);
            else if (key == "amount") amount = std::stoi(value);
        }
    }
    FontKerning kerning(first, second, amount);

    m_font.AddKerning(kerning);
}
//Cleans up the line and returns the clean line
std::string ProcessFiles::TrimLine(std::string line){
    if (line.empty()) return line;
    // Trim whitespace
    line.erase(0, line.find_first_not_of(" \t"));
    line.erase(line.find_last_not_of(" \t") + 1);
    return line;
}
