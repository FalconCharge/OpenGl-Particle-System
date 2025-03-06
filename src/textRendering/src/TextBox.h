#pragma once

#include <string>
#include <vector>
#include "Font.h"
#include "Quad.h"
#include "ProcessFile.h"

class TextBox {
public:
    // Constructor
    TextBox(const std::string& processFileName, const std::string& text, float width, float height);
    ~TextBox();

    // Settters
    void SetPosition(glm::vec2 pos){m_vPosition = pos; GenerateQuads();}
    void SetSize(glm::vec2 size){m_vSize = size; GenerateQuads();}
    //void setCharSpacing(float spacing){m_charSpacing = spacing;}  // Not implemented
    void setDisplayText(std::string displayText){m_sDisplayText = displayText; GenerateQuads();}
    void setFont(Font* font){m_font = font;}
    void setColor(glm::vec4 color){m_vColor = color;}
    void SetHorizontalAlignment(std::string allignment);
    void SetBackroundColor(glm::vec4 color){m_vBackroundColor = color;}
    void SetSpaceSize(float size){m_fSpaceWidth = size;}
    

    //Getters
    glm::vec2 getPosition(){return m_vPosition;}
    glm::vec2 getSize(){return m_vSize;}
    Font* getFont(){return m_font;}
    std::string getDisplayText(){return m_sDisplayText;}
    float getCharSpacing(){return m_charSpacing;}

    void Init();

    // Render the text
    void render(int width, int height);

    void DebugTextBox(){
        // Print out debug information about the TextBox
        std::cout << "TextBox Debug Information:\n";
        std::cout << "Position: (" << m_vPosition.x << ", " << m_vPosition.y << ")\n";
        std::cout << "Size: (" << m_vSize.x << ", " << m_vSize.y << ")\n";
        std::cout << "Text: " << m_sDisplayText << "\n";
        std::cout << "Character Spacing: " << m_charSpacing <<" Dont think this is used I hope" <<"\n";
        std::cout << "Color: " << m_vColor.r << ", " << m_vColor.g << ", " << m_vColor.b << std::endl;
        std::cout << "Font: " << (m_font ? "Set" : "Not Set") << "\n";
        std::cout << "Number of Characters: " << m_quads.size() << "\n";
    }

private:

    glm::vec2 m_vPosition = glm::vec2(0.0f, 0.0f);      // Position of the TextBox
    glm::vec2 m_vSize = glm::vec2(25.0f, 25.0f);        // Size of the textBox
    glm::vec4 m_vColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // Default text color
    glm::vec4 m_vBackroundColor = glm::vec4(0, 0, 1, 0.5);

    std::string m_sDisplayText = "Enter text to display...";    //Text inside box
    std::string m_sCleanDisplayText = "";                       //Used to delete spaces
    float m_charSpacing = 1.0f;        // spacing between chars
    std::string m_sAllignment = "AL_Left";  //Sets the allignment

    Font* m_font;       //Stores the current font

    std::vector<Quad*> m_quads;    // Quads for each character
    Quad* m_backround;  //Stores the backround quad

    // Holds the shaders that are used
    wolf::Program* m_pProgram = nullptr;
    wolf::Program* m_pProgramBackround = nullptr;

    float m_fSpaceWidth;    //Used to store the width of a space. I Implemented this wrong here so now you have the option to change the space width

    // Generate quads based on the text and font
    void GenerateQuads();
    // Creats a backround for the textbox
    void GenerateBackround();

    // Hold the positions of the quads
    std::vector<glm::vec2> CalculateQuadPositions();


    //Helper Funciton
    bool isWord(std::string word, char nextChar);
    float CalculateWordWidth(const std::string& wordBlock, char previousChar);
    float TextBox::CalculateLineWidth(std::string& line);
    void CalculateLineXPosition(float lineWidth, glm::vec2 textboxSize, float &currentX);   //Caculates the position on where the line should start at
    void AddTextures();

    ProcessFiles* m_pProcessFile;

    //Used to generate the vertices and update them
    void SetupVerts();
    void SetupVertsInit();
    //Debug 
    void PrintVertices(std::vector<Vertex> &quadVertices);

    //A map of the verts we will render in a specific order
    std::unordered_map<int, std::vector<Vertex>> m_vertices;

    //Array for differnt VB and dec for rendering with differnt texures 
    std::unordered_map<int, wolf::VertexBuffer*> m_VBquad;
    std::unordered_map<int, wolf::VertexDeclaration*> m_pDecl;
    //A map of the textures we will use to render
    std::unordered_map<int, wolf::Texture*> m_textures;
};
