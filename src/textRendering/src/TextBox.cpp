#include "TextBox.h"

//Constructor
TextBox::TextBox(const std::string& processFileName, const std::string& text, float width, float height)
    : m_sDisplayText(text), m_vSize(width, height), m_pProcessFile(new ProcessFiles(processFileName))
{
    m_pProgram = wolf::ProgramManager::CreateProgram("src/textRendering/Data/Shaders/Quad.vsh", "src/textRendering/Data/Shaders/Quad.fsh");
    m_font = m_pProcessFile->getFont();

    //Sets the size of the space value
    SetSpaceSize(m_font->GetxAdvance(' '));

}
// Deconstructor
TextBox::~TextBox() {

    // Cleanup dynamically allocated quads
    for (auto& quad : m_quads) {
        delete quad;  // Delete each dynamically allocated Quad
    }
    m_quads.clear();  // Clear the vector of quads

    // Cleanup textures stored in m_textures (if dynamically allocated)
    for (auto& texturePair : m_textures) {
        wolf::Texture* texture = texturePair.second;
        if (texture) {
            //std::cout << "Deleting texture for texture ID: " << texturePair.first << std::endl;
            wolf::TextureManager::DestroyTexture(texture);  // Delete each dynamically allocated texture
        }
    }
    m_textures.clear();  // Clear the texture map

    // Cleanup vertex buffers and declarations
    for (auto& vbPair : m_VBquad) {
        //std::cout << "Destroying vertex buffer for texture ID: " << vbPair.first << std::endl;
        wolf::BufferManager::DestroyBuffer(vbPair.second);  // Destroy the vertex buffer
    }
    m_VBquad.clear();  // Clear the vertex buffer map

    for (auto& declPair : m_pDecl) {
        //std::cout << "Deleting vertex declaration for texture ID: " << declPair.first << std::endl;
        delete declPair.second;  // Delete the vertex declaration
    }
    m_pDecl.clear();  // Clear the vertex declaration map

    std::cout << "TextBox destruction complete." << std::endl;
}
// The render fucntion for the textBox
void TextBox::render(int width, int height) {
    if (m_pProgram == nullptr) {
        std::cerr << "Error: Shader program is not initialized.\n";
        return;
    }

    //Render the backround first
    m_backround->render(width, height);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set up projection and view matrices
    float fWidth = (float)width;
    float fHeight = (float)height;

    glm::mat4 mProj = glm::ortho(0.0f, fWidth, 0.0f, fHeight, -1.0f, 1.0f);       // Screen dimensions
    glm::mat4 mView = glm::mat4(1.0f);                                              // Identity matrix for view
    glm::mat4 model = glm::mat4(1.0f);                                              // Identity matrix for model
    
    glm::mat4 mvp = mProj * mView * model;  // Model-View-Projection matrix

    // Bind the shader program
    m_pProgram->Bind();
    m_pProgram->SetUniform("MVP", mvp);
    m_pProgram->SetUniform("color", m_vColor);

    // Iterate through each font page (unordered_map) and render them
    // Render all texture groups
    for (auto& it : m_VBquad) {
        int textureID = it.first;
        auto& vb = it.second;
        // Bind the texture from m_textures
        if (m_textures.find(textureID) != m_textures.end()) {
            m_textures[textureID]->Bind();
        } else {
            std::cerr << "Error: Missing texture for texture ID " << textureID << ".\n";
            continue; // Skip rendering for this texture group
        }

        m_pProgram->SetUniform("tex", 0);

        // Bind the vertex declaration for this texture group
        m_pDecl[textureID]->Bind();

        // Draw all vertices for this texture group
        size_t vertexCount = m_vertices[textureID].size();
        if (vertexCount == 0) {
            std::cerr << "Error: No vertices found for texture ID " << textureID << ".\n";
            continue;
        }
        //Draw the first bunch of quads
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    }
    

}
// The basic Inits for the textbox
void TextBox::Init(){
    
    GenerateBackround();
    GenerateQuads();
    AddTextures();
}
// Adds the textures into a mapping later used in rendering
void TextBox::AddTextures()
{
    // Loop through all quads and add their textures to m_textures
    for (auto &quad : m_quads)
    {
        int textureID = quad->getTextureID(); // Retrieve the texture ID for the quad

        // Check if the textureID is already in m_textures
        if (m_textures.find(textureID) == m_textures.end())
        {
            std::string fileName = m_font->GetPages()[textureID].fileLocation;
            wolf::Texture* newTexture = wolf::TextureManager::CreateTexture("src/textRendering/Data/Textures/" + fileName);
            m_textures[textureID] = newTexture; // Store the texture in the unordered_map
        }
    }
}
// Setups the verts on the first run We don't actually use this function but I'm too scared to delete it
void TextBox::SetupVertsInit(){
    std::cout << "setting up verts" << std::endl;
    //Delete the old VB and Declartion
    for (auto& it : m_pDecl) {
        delete it.second;
        std::cout << "Deleting declaration for texture ID: " << it.first << "\n";
    }
    for (auto& it : m_VBquad) {
                std::cout << "Destroying buffer for texture ID: " << it.first << "\n";
        wolf::BufferManager::DestroyBuffer(it.second);
    }

    m_pDecl.clear();
    m_VBquad.clear();
    m_vertices.clear();

    for (auto& quad : m_quads) {
        //Using the page ID to reference the texture
        std::vector<Vertex> quadVertices = quad->GetVertices();

        int textureID = quad->getTextureID();

        std::cout << "Quad with texture ID: " << textureID << " has " << quadVertices.size() << " vertices.\n";


        m_vertices[textureID].insert(m_vertices[textureID].end(), quadVertices.begin(), quadVertices.end());

    }
     std::cout << "Creating vertex buffers and declarations for each texture group...\n";
    // Create Vertex Buffers and Declarations for each texture group
    for (auto& it : m_vertices) {
        int textureID = it.first;
        auto& vertices = it.second;

        std::cout << "Creating buffer for texture ID: " << textureID << " with " << vertices.size() << " vertices.\n";

        // Create a vertex buffer for this texture group
        m_VBquad[textureID] = wolf::BufferManager::CreateVertexBuffer(vertices.data(), vertices.size() * sizeof(Vertex));

        std::cout << "Created vertex buffer for texture ID: " << textureID << "\n";

        // Create a corresponding vertex declaration
        m_pDecl[textureID] = new wolf::VertexDeclaration();
        m_pDecl[textureID]->Begin();
        m_pDecl[textureID]->AppendAttribute(wolf::AT_Position, 2, wolf::CT_Float);
        m_pDecl[textureID]->AppendAttribute(wolf::AT_TexCoord1, 2, wolf::CT_Float);
        m_pDecl[textureID]->SetVertexBuffer(m_VBquad[textureID]);
        m_pDecl[textureID]->End();

        std::cout << "Created vertex declaration for texture ID: " << textureID << "\n";
    }

    std::cout << "Vertex buffers and declarations setup completed.\n";
}
// Debug function
void TextBox::PrintVertices(std::vector<Vertex> &quadVertices)
{
    // Print the vertex data (positions and texcoords)
    for (const auto &vertex : quadVertices)
    {
        std::cout << "Position: ("
                  << vertex.x << ", "
                  << vertex.y << ") | "
                  << "TexCoord: ("
                  << vertex.u << ", "
                  << vertex.v << ")\n";
    }
}
//Calls each time we make a adjustment to positions or scale and rebuilds the verts
void TextBox::SetupVerts() {

    m_pDecl.clear();
    m_VBquad.clear();
    m_vertices.clear();

    // Check if any quads are dirty
    bool anyDirty = false;
    for (auto& quad : m_quads) {
        if (quad->isDirty()) {
            anyDirty = true;
            break; // Stop early if we find a dirty quad
        }
    }

    if (anyDirty) {
        //Delete the old VB and Declartion
        for (auto& it : m_pDecl) {
            delete it.second;
        }
        for (auto& it : m_VBquad) {
            wolf::BufferManager::DestroyBuffer(it.second);
        }


        // Rebuild vertices grouped by texture ID
        for (auto& quad : m_quads) {
            std::vector<Vertex> quadVertices = quad->GetVertices();
            //PrintVertices(quadVertices);
            
            
            int textureID = quad->getTextureID();
            m_vertices[textureID].insert(m_vertices[textureID].end(), quadVertices.begin(), quadVertices.end());
        }

        // Create buffers and declarations for each texture group
        for (auto& it : m_vertices) {
            int textureID = it.first;
            auto& vertices = it.second;


            if (!vertices.empty()) { // Ensure non-empty vertices
                m_VBquad[textureID] = wolf::BufferManager::CreateVertexBuffer(vertices.data(),vertices.size() * sizeof(Vertex));

                m_pDecl[textureID] = new wolf::VertexDeclaration();
                m_pDecl[textureID]->Begin();
                m_pDecl[textureID]->AppendAttribute(wolf::AT_Position, 2, wolf::CT_Float);
                m_pDecl[textureID]->AppendAttribute(wolf::AT_TexCoord1, 2, wolf::CT_Float);
                m_pDecl[textureID]->SetVertexBuffer(m_VBquad[textureID]);
                m_pDecl[textureID]->End();
            }
        }
    }
}

// Sets all the values for the backround quad
void TextBox::GenerateBackround(){
    m_pProgramBackround = wolf::ProgramManager::CreateProgram("src/textRendering/Data/Shaders/Quad.vsh", "src/textRendering/Data/Shaders/TextBoxBackround.fsh");
    //Create a quad and set size as the textbox
    m_backround = new Quad();
    m_backround->setPosition(this->getPosition());
    m_backround->setScale(glm::vec2(this->getSize().x, -(this->getSize().y)));
    m_backround->setColor(m_vBackroundColor);
    m_backround->setShader(m_pProgramBackround);
    m_backround->SetName('_');
    m_backround->SetTexture(nullptr);
}
// Caculates the quad position and returns a vector storing to be later used when building the quads
std::vector<glm::vec2> TextBox::CalculateQuadPositions() {
    std::vector<glm::vec2> positions;

    int lineHeight = m_font->GetCommonLineHeight();
    int baseline = m_font->GetBase();

    float startingX = getPosition().x + 2.0f;   //Magic number to have the text not generate on the wall //
    //Line Is not need I think

    float startingY = getPosition().y - lineHeight; //skips the first line

    float currentX = startingX;
    float currentY = startingY;

    glm::vec2 textboxSize = getSize();


    char previousChar = '\0';
    std::string wordBlock = "";
    m_sCleanDisplayText = "";

    // Temporary container to hold line data for allignment
    std::vector<std::string> lines;
    float currLineWidth = 0;

    float counter = 0;

    //Add a space at the end so we consider it a word
    if(m_sDisplayText.back() != ' '){
        m_sDisplayText += ' ';
    }

    //Loops through all the chars in the display text
    for (size_t i = 0; i < m_sDisplayText.size(); ++i) {
        char c = m_sDisplayText[i];

        if(isWord(wordBlock, c)){

            float wordWidth = CalculateWordWidth(wordBlock, previousChar);

            //Add the space into the width
            wordWidth += m_fSpaceWidth;
            
            if(lines.empty() || currLineWidth + wordWidth > textboxSize.x){
                currLineWidth = 0;
                lines.push_back(wordBlock);
                wordBlock.clear();
                currLineWidth += wordWidth;
            }else{
                lines.back() += ' ';
                lines.back() += wordBlock;
                wordBlock.clear();
                currLineWidth += wordWidth;
                currLineWidth += m_fSpaceWidth;
            }
        }else{
            //If it ain't a space add it in to the wordBlock
            wordBlock += c;
        }
        

    }

    // Setting positions

    for(size_t lineIndex = 0; lineIndex < lines.size(); ++lineIndex){
        std::string line = lines[lineIndex];
        float lineWidth = CalculateLineWidth(line);

        // Determine centering
        CalculateLineXPosition(lineWidth, textboxSize, currentX);


        for(char c : line){
            int charID = c;
            int yOffset = m_font->GetYOffset(charID);


            // Apply kerning offset
            if (previousChar != '\0') {
                int kerning = m_font->GetKerning(previousChar, c);
                currentX += kerning;  // Apply kerning offset to position
            }

            positions.emplace_back(currentX, currentY + baseline - yOffset - m_font->GetHeight(charID));

            
            if(c == ' '){
                currentX += m_fSpaceWidth;
            }else{
                currentX += m_font->GetWidth(c);
            }
            
            
            previousChar = c;
        }
        currentY -= lineHeight;
        previousChar = '\0';    // Reset for the next line
    }
    counter = 0;
    return positions;
}
//Using the positions vector We add the quads into m_quads
void TextBox::GenerateQuads() {
    int counter = 0;
    // Clear old quads
    m_quads.clear();

    // Get positions for each character
    std::vector<glm::vec2> positions = CalculateQuadPositions();

    glm::vec2 textureSize = m_font->GetTextureSize();
    char previousChar = '\0';


    size_t charIndex = 0;
    for (char c : m_sCleanDisplayText) {    //replace display text with clean (So we don't go out of range!)

        float uStart, uEnd, vStart, vEnd;
        m_font->GetExtentsForChar(c, &uStart, &uEnd, &vStart, &vEnd);

        float width = m_font->GetWidth(c);
        float height = m_font->GetHeight(c);

        // Create and configure quad
        Quad* quad = new Quad();
        quad->setColor(m_vColor);
        quad->SetTextureCoordinates(uStart, vStart, uEnd, vEnd);
        quad->setPosition(positions[charIndex]);
        quad->setScale(glm::vec2(width, height));
        quad->SetTexture(m_font->GetTextureForChar(c));
        quad->SetTextureID(m_font->GetPageID(c));
        quad->SetName(c);
        //quad->DebugInfo();

        m_quads.push_back(quad);
        
        if(charIndex > positions.size() -1){
            std::cerr << "M_displayText has more values then positions ERROR!!" << std::endl;
            std::cerr << "breaking loop" << std::endl;
            break;
        }
        ++charIndex;

        previousChar = c;
    }
    //Each time we generate new quads we setup the new verts
    SetupVerts();
    //this->DebugTextBox();
}
// Sets the horizontalAllignment based on input
void TextBox::SetHorizontalAlignment(std::string alignment) {
    if (alignment == "AL_Center") {
        m_sAllignment = alignment;
    } else if (alignment == "AL_Left") {
        m_sAllignment = alignment;
    } else {
        m_sAllignment = "AL_Right";
    }
}
//Using the chars We calculate the total width of the wordblock
float TextBox::CalculateWordWidth(const std::string& wordBlock, char previousChar) {
    char previousChar1 = '\0';
    float wordWidth = 0.0f;
    for (char wc : wordBlock) {
        if(previousChar1 != '\0'){
            wordWidth += m_font->GetKerning(previousChar1, wc);
        }
        if(wc == ' '){
            wordWidth += m_fSpaceWidth;
        }else{
            wordWidth += m_font->GetWidth(wc);
        }

        previousChar1 = wc;
    }
    return wordWidth;
}
// Calculates the line widht of a string using information from m_font
float TextBox::CalculateLineWidth(std::string& line) {


    char previousChar = '\0';

    // Create a trimmed string for editing
    std::string trimmedLine = line;

    // Calculate width
    float lineWidth = 0.0f;
    for (char lc : trimmedLine) {
        if(lc == ' '){
            lineWidth += m_fSpaceWidth;
        }else{
            lineWidth += m_font->GetWidth(lc);
        }
        
        // Apply kerning
        if (previousChar != '\0') {
            lineWidth += m_font->GetKerning(previousChar, lc);
        }
        
        previousChar = lc; // Update previous character for kerning
    }

    //Creats a new text to display without the spaces at the end or start
    m_sCleanDisplayText += trimmedLine;
    return lineWidth;
}
// Using the alignment value caculate the position of the text
void TextBox::CalculateLineXPosition(float lineWidth, glm::vec2 textboxSize, float &currentX) {
    if (m_sAllignment == "AL_Center") {
        currentX = getPosition().x + (textboxSize.x - lineWidth) / 2.0f;
    } else if (m_sAllignment == "AL_Right") {
        currentX = getPosition().x + textboxSize.x - lineWidth;
    } else { // Default to left alignment
        currentX = getPosition().x;
    }
}
//Checks if teh string is a full word of not (depending on if the next char is a space)
bool TextBox::isWord(std::string word, char nextLetter){
    if(word == ""){
        return false;
    }
    if(nextLetter == ' '){
        return true;
    }
    return false;

}