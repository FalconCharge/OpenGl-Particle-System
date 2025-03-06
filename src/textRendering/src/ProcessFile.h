#pragma once

#include "Font.h"  // Include the Font class definition

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

//Process Files is a supporting class to Font where It's use is to process the information and add it to font
class ProcessFiles {
public:
    //Starts processing the file name entered
    ProcessFiles(const std::string fileName);

    //Returns the font processfile is connect to
    Font* getFont()  { return &m_font; }

    //Parse the current line
    void parseCharacterLine(const std::string& line);


private:
    //Process the file
    void ProcessFile(std::string fileName);
    //Process the currentLine
    void ProcessLine(std::string line);
    //Cleanup the line
    std::string TrimLine(std::string line);

    // Helper methods to parse specific lines
    //void parseCharacterLine(const std::string& line);
    void parseInfoLine(const std::string& line);
    void parseCommonLine(const std::string& line);
    void parsePageLine(const std::string& line);
    void parseKerningLine(const std::string& line);

    Font m_font;  // Font instance for storing parsed data
};
