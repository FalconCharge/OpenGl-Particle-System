#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include "../../wolf/wolf.h"

//Struct that stores character information
struct Character {
    int id;
    int x, y;     
    int width, height; 
    int xAdvance, xOffset, yOffset; 
    int page, chnl;
    // Constructor for Character struct
    Character(int id = 0, int x = 0, int y = 0, int width = 0, int height = 0,
              int xAdvance = 0, int xOffset = 0, int yOffset = 0, 
              int page = 0, int chnl = 0) 
        : id(id), x(x), y(y), width(width), height(height), 
          xAdvance(xAdvance), xOffset(xOffset), yOffset(yOffset), 
          page(page), chnl(chnl) {}
};
//Struct that stores the info line for the font
struct Info {
    std::string face;
    int size;
    bool bold;
    bool italic;
    std::string charset;
    bool unicode;
    int stretchH;
    bool smooth;
    int aa;
    int padding[4];
    int spacing[2];
    int outline;

    Info()
        : face(""), size(0), bold(false), italic(false), charset(""),
          unicode(false), stretchH(0), smooth(false), aa(0), outline(0) {
        // Initialize arrays explicitly
        for (int i = 0; i < 4; ++i) padding[i] = 0;
        for (int i = 0; i < 2; ++i) spacing[i] = 0;
    }

};
// Struct that stores the common line for the font
struct Common {
    int lineHeight;
    int base;
    int scaleW;
    int scaleH;
    int pages;
    bool packed;
    int alphaChnl;
    int redChnl;
    int greenChnl;
    int blueChnl;
    
    Common()
        : lineHeight(0), base(0), scaleW(0), scaleH(0), pages(0), 
          packed(false), alphaChnl(0), redChnl(0), greenChnl(0), blueChnl(0) {}

};
//Struct that stores the page line from the font
struct Page{
    int id;
    std::string fileLocation;

    Page(int i, std::string f) : id(i), fileLocation(f) {}
    Page(){}
};
// Storage for the kernign of each font
struct FontKerning {
    int first;   // The first character ID
    int second;  // The second character ID
    int amount;  // The kerning adjustment value

    FontKerning(int f, int s, int a) : first(f), second(s), amount(a) {}
};


//A font class which stores information obtained from font files
class Font {
public:
    Font();
    ~Font();

    //Adds a character to the character map
    void AddCharacter(const Character& ch);
    //Adds the common line to the common struct
    void AddCommon(const Common& common);
    //Adds a kerning to the kerning list
    void AddKerning(const FontKerning& kerning);
    //Adds a page to the page list
    void AddPage(const Page& page);
    //Adds the Info line into the Info struct
    void AddInfo(const Info& info);

    //Returns the size of the font
    int GetSize(){return m_pInfo->size;}
    //returns a vec2 of the texture size
    glm::vec2 GetTextureSize(){return glm::vec2(m_pCommon->scaleW, m_pCommon->scaleH);}
    //returns that int value of a kerning pair
    int GetKerning(char prev, char curr);
    //Returns the Y Offset from a certain char
    int GetYOffset(int charID){return characters[charID].yOffset;}
    //Returns the height of a certain char
    int GetHeight(int charID){return characters[charID].height;}
    // Returns the Width of a certain char
    int GetWidth(int charID){return characters[charID].width;}
    // Returns the xAdavnce of a certain char
    int GetxAdvance(int charID){return characters[charID].xAdvance;}
    // Returns the commonLine Height for the font
    int GetCommonLineHeight(){return m_pCommon->lineHeight;}
    // Returns the Base Line from the font
    int GetBase(){return m_pCommon->base;}
    // Returns a the page mapping
    std::unordered_map<int, Page> GetPages(){return pages;}
    //Returns the pageID from a char
    int GetPageID(int charId);

    // Gets the texture for the char
    wolf::Texture* GetTextureForChar(int charId);
    //Adjust the values in the parameter and sets them to the proper UV values
    void GetExtentsForChar(int charId, float* uStart, float* uEnd, float* vStart, float* vEnd);

    //Debug class info
    void PrintFontDetails();
    void PrintFontDetails1() const;

private:
    //Storage for the characters
    std::unordered_map<int, Character> characters;
    //Storage for the Info
    Common* m_pCommon = nullptr;
    //Storage for the pages
    std::unordered_map<int, Page> pages; //Using unordered map becuase they have IDs
    //Storage for the Info
    Info* m_pInfo = nullptr;
    //Storage for the kernings
    std::vector<FontKerning> m_kerningList;

};