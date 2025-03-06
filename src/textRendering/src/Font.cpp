#include "Font.h"


Font::Font() : m_pCommon(nullptr), m_pInfo(nullptr){}

Font::~Font(){
    characters.clear();
    pages.clear();
    delete m_pCommon;
    delete m_pInfo;
}

//Returns a texture with the character on it
wolf::Texture* Font::GetTextureForChar(int charId) {
    int pageID = characters[charId].page;
    std::string fullPath = "src/textRendering/Data/Textures/" + pages[pageID].fileLocation;
    wolf::Texture* p_tex = wolf::TextureManager::CreateTexture(fullPath);
    return p_tex;
}
//returns the ID of the page the char is connected to
int Font::GetPageID(int charID){
    int pageID = characters[charID].page;
    return pages[pageID].id;
}
//Returns the UV positions for a certian char
void Font::GetExtentsForChar(int charId, float* uStart, float* uEnd, float* vStart, float* vEnd) {
    Character ch = characters[charId];

    *uStart = static_cast<float>(ch.x) / m_pCommon->scaleW;
    *uEnd = static_cast<float>(ch.x + ch.width) / m_pCommon->scaleW;
    
    *vStart = static_cast<float>(ch.y) / m_pCommon->scaleH;
    *vEnd = static_cast<float>(ch.y + ch.height) / m_pCommon->scaleH;
    return;
};

// Adds a character to the Font 
void Font::AddCharacter(const Character& ch) {
    int customKey = ch.id;  
    characters.emplace(customKey, ch);
}

// AddCommon method
void Font::AddCommon(const Common& common) {
    if (!m_pCommon) {
        m_pCommon = new Common(common);  // Dynamically allocate if necessary
    } else {
        *m_pCommon = common;  // Or overwrite if already exists
    }
}

// AddKerning method
void Font::AddKerning(const FontKerning& kerning) {
    m_kerningList.push_back(kerning);
}

// Goes through a until it finds it's kerning value if not found return 0
int Font::GetKerning(char inLastChar, char inCurrentChar){
    //Could improve this function with a different data structure
    int lastChar = inLastChar;      //Ids of last char
    int currChar = inCurrentChar;   //Ids of current char

    //If the last char was a space do + 2
    if(lastChar == ' '){
        return 4;
    }

    //Go through the list and find if a value matches
    for (auto& value : m_kerningList){

        if(value.first == lastChar && value.second == currChar){
            return value.amount;
        }
    }
    return 0;
}
//Adds a page to a map
void Font::AddPage(const Page& page) {
    // Manually specify the key some reason
    int customKey = page.id;
    pages.emplace(customKey, page);
}

// AddInfo method
void Font::AddInfo(const Info& info) {
    if (!m_pInfo) {
        m_pInfo = new Info(info);  // Dynamically allocate if necessary
    } else {
        *m_pInfo = info;  // Or overwrite if already exists
    }
}
// Debug method to print font details
void Font::PrintFontDetails1() const {
    std::cout << "Font Info: " << (m_pInfo ? m_pInfo->face : "No info") << std::endl;
    std::cout << "Common: " << (m_pCommon ? "Exists" : "No common data") << std::endl;
    std::cout << "Number of characters: " << characters.size() << std::endl;
    std::cout << "Number of pages: " << pages.size() << std::endl;
    std::cout << "Number of kernings: " << m_kerningList.size() << std::endl;
}
// Debug method to print font details
void Font::PrintFontDetails() {
    // Printing Characters
    std::cout << "Characters:" << std::endl;
    std::cout << this->characters.size() << std::endl;
    for (const auto& pair : this->characters) {
        const Character& ch = pair.second;
        std::cout << "ID: " << ch.id << ", X: " << ch.x << ", Y: " << ch.y
                  << ", Width: " << ch.width << ", Height: " << ch.height
                  << ", XAdvance: " << ch.xAdvance << ", XOffset: " << ch.xOffset
                  << ", YOffset: " << ch.yOffset << ", Page: " << ch.page
                  << ", Channel: " << ch.chnl << std::endl;
    }

    // Printing Common Info
    if (this->m_pCommon) {
        const Common& common = *this->m_pCommon;
        std::cout << "Common Info:" << std::endl;
        std::cout << "LineHeight: " << common.lineHeight << ", Base: " << common.base
                  << ", ScaleW: " << common.scaleW << ", ScaleH: " << common.scaleH
                  << ", Pages: " << common.pages << ", Packed: " << common.packed
                  << ", AlphaChnl: " << common.alphaChnl
                  << ", RedChnl: " << common.redChnl
                  << ", GreenChnl: " << common.greenChnl
                  << ", BlueChnl: " << common.blueChnl << std::endl;
    } else {
        std::cout << "No Common Info Available" << std::endl;
    }

    // Printing Pages
    std::cout << "Pages:" << std::endl;
    for (const auto& pair : this->pages) {
        const Page& page = pair.second;
        std::cout << "ID: " << page.id << ", File Location: " << page.fileLocation << std::endl;
    }

    // Printing Info
    if (this->m_pInfo) {
        const Info& info = *this->m_pInfo;
        std::cout << "Font Info:" << std::endl;
        std::cout << "Face: " << info.face << ", Size: " << info.size
                  << ", Bold: " << info.bold << ", Italic: " << info.italic
                  << ", Charset: " << info.charset << ", Unicode: " << info.unicode
                  << ", StretchH: " << info.stretchH << ", Smooth: " << info.smooth
                  << ", AA: " << info.aa
                  << ", Padding: {" << info.padding[0] << ", " << info.padding[1]
                  << ", " << info.padding[2] << ", " << info.padding[3] << "}, "
                  << "Spacing: {" << info.spacing[0] << ", " << info.spacing[1] << "}, "
                  << "Outline: " << info.outline << std::endl;
    } else {
        std::cout << "No Font Info Available" << std::endl;
    }

    // Printing Kernings
    std::cout << "Kernings:" << std::endl;
    for (const auto& k : m_kerningList) {
            std::cout << "First: " << k.first << ", Second: " << k.second << ", Amount: " << k.amount << std::endl;
        }
}

