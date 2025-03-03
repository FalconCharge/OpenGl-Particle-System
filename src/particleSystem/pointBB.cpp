#include "pointBB.h"

#include <iostream>
#include <string>

PointBB::PointBB(){

}
PointBB::PointBB(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation){

}
PointBB::PointBB(glm::vec3 position){
    
}
PointBB::~PointBB(){
    delete m_pDecl;
    //wolf::BufferManager::DestroyBuffer(m_pIB);
    wolf::BufferManager::DestroyBuffer(m_pVB);
    wolf::ProgramManager::DestroyProgram(m_pProgram);
    wolf::TextureManager::DestroyTexture(m_pTexture);

    normalVertices.clear();
    //cubeIndices.clear();
}

void PointBB::Init(){
    //std::cout << "Initlizing: " << GetName() << std::endl;
    //renderBB();
    if(m_pProgram == nullptr){
        m_pProgram = wolf::ProgramManager::CreateProgram("Data/Shader/DebugCube.vsh", "Data/Shader/DebugCube.fsh");
    }
    if(m_pTexture == nullptr){
        m_pTexture = wolf::TextureManager::CreateTexture("Data/textures/muzzle_04.png");
    }

    if(normalVertices.empty()){
        normalVertices = {
            // Center point at (0, 0, 0)
            {0.0f, 0.0f, 0.0f},
            // Points along the X axis
            { 1.0f, 0.0f, 0.0f}, // Right
            {-1.0f, 0.0f, 0.0f}, // Left
    
            // Points along the Y axis
            {0.0f,  1.0f, 0.0f}, // Up
            {0.0f, -1.0f, 0.0f}, // Down
    
            // Points along the Z axis
            {0.0f, 0.0f,  1.0f}, // Forward
            {0.0f, 0.0f, -1.0f}  // Backward
        };
    }
    
    
    

    //Create VB & IB & VertexDeclation

    m_pVB = wolf::BufferManager::CreateVertexBuffer(normalVertices.data(), normalVertices.size() * sizeof(PointBBVertex));

    m_pDecl = new wolf::VertexDeclaration();

    m_pDecl->Begin();
    m_pDecl->AppendAttribute(wolf::AT_Position, 3, wolf::CT_Float);     // Pos attr
    m_pDecl->SetVertexBuffer(m_pVB);
    m_pDecl->End();

    


}
/*
void PointBB::Render(glm::mat4 proj, glm::mat4 view){

    glEnable(GL_PROGRAM_POINT_SIZE);    // Enables point for particles I think This makes it better to render? like less draw calls



    Node::Render(proj, view);

    
    // ERROR IN GetWorldTransform somewhere but I gotta finish this assignment
    // So I'm just going to work around it 
    // Typically we would pass in the transform but instead we are going to make a new transform
    // That has the correct values
    
    
    glm::mat4 world = glm::mat4(1.0f);
    world = glm::translate(world, this->GetWorldPosition());    

    glm::mat4 MVP = proj * view * world;

    // Use shader program.
    if(m_pProgram == nullptr){
        std::cout << "Shader not added to DebugCube!" << std::endl;
        return;
    }

    m_pProgram->Bind();

    m_pProgram->SetUniform("MVP", MVP);

    m_pProgram->SetUniform("color", getColor());
    //Cool effect kinda represent speakers by shaking
    //m_pProgram->SetUniform("size", (rand() % 1000) + 1000.0f);
    m_pProgram->SetUniform("size", 1000.0f);


    if(m_pTexture){
        m_pTexture->Bind(0);
        m_pProgram->SetUniform("tex", 0);
    }
    

    m_pDecl->Bind();

    glDrawArrays(GL_POINTS, 0, normalVertices.size());

}*/
void PointBB::SetTexture(const std::string& p_sTexturePath){
    m_pTexture = wolf::TextureManager::CreateTexture(p_sTexturePath);
    
    m_pTexture->SetFilterMode(wolf::Texture::FM_Linear, wolf::Texture::FM_LinearMipmap);
    m_pTexture->SetWrapMode(wolf::Texture::WM_Repeat, wolf::Texture::WM_Repeat);
}
void PointBB::SetShader(const std::string& p_sVshPath, const std::string& p_sFshPath){
    m_pProgram = wolf::ProgramManager::CreateProgram(p_sVshPath, p_sFshPath);
}

void PointBB::UpdateVerts(){

}

void PointBB::SetVertices(std::vector<PointBBVertex> p_normalVertices){
    normalVertices = p_normalVertices;
}