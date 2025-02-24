#include "billBoard.h"

#include <iostream>
#include <string>

BillBoard::BillBoard(){

}
BillBoard::BillBoard(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation){
    SetLocalScale(scale);
    SetLocalPosition(position);
    SetLocalRotation(rotation);
}
BillBoard::~BillBoard(){
    delete m_pDecl;
    wolf::BufferManager::DestroyBuffer(m_pIB);
    wolf::BufferManager::DestroyBuffer(m_pVB);
    wolf::ProgramManager::DestroyProgram(m_pProgram);
    wolf::TextureManager::DestroyTexture(m_pTexture);

    normalVertices.clear();
    cubeIndices.clear();
}

void BillBoard::Init(){
    SetDirty(true);
    //std::cout << "Initlizing: " << GetName() << std::endl;
    //renderBB();
    if(m_pProgram == nullptr){
        m_pProgram = wolf::ProgramManager::CreateProgram("Data/Shader/DebugCube.vsh", "Data/Shader/DebugCube.fsh");
    }
    // Cube vertex data with texture coordinates and normals CHATGPT GENERATED
    // I'm guessing a better option like creating serperate vectors for pos, uvs, normals just to make it more clean (add them together)
    normalVertices = {
        // Positions        // Texture Coords  // Normal (pointing forward)
        { -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,    0.0f, 0.0f, 1.0f }, // Bottom-left
        {  0.5f, -0.5f, 0.0f,   1.0f, 0.0f,    0.0f, 0.0f, 1.0f }, // Bottom-right
        {  0.5f,  0.5f, 0.0f,   1.0f, 1.0f,    0.0f, 0.0f, 1.0f }, // Top-right
        { -0.5f,  0.5f, 0.0f,   0.0f, 1.0f,    0.0f, 0.0f, 1.0f }  // Top-left
    };
    
    // Indices for two triangles forming a quad
    cubeIndices = {
        0, 1, 2,  // Triangle 1
        0, 2, 3   // Triangle 2
    };
    

    //Create VB & IB & VertexDeclation

    m_pVB = wolf::BufferManager::CreateVertexBuffer(normalVertices.data(), normalVertices.size() * sizeof(BillBoardVertex));
    m_pIB = wolf::BufferManager::CreateIndexBuffer(cubeIndices.data(), cubeIndices.size());

    m_pDecl = new wolf::VertexDeclaration();

    m_pDecl->Begin();
    m_pDecl->AppendAttribute(wolf::AT_Position, 3, wolf::CT_Float);     // Pos attr
    m_pDecl->AppendAttribute(wolf::AT_TexCoord1, 2, wolf::CT_Float);    // Texture attr
    m_pDecl->AppendAttribute(wolf::AT_Normal, 3, wolf::CT_Float);       // Normal attr
    m_pDecl->SetVertexBuffer(m_pVB);
    m_pDecl->SetIndexBuffer(m_pIB);
    m_pDecl->End();

    

    SetVolume(CalculateVolume());

}
void BillBoard::Update(float p_fDelta){
    Node::Update(p_fDelta);   // Not sure if this should be here or not

    FaceCamera();
}
void BillBoard::Render(glm::mat4 proj, glm::mat4 view){
    Node::Render(proj, view);

    /* 
    // ERROR IN GetWorldTransform somewhere but I gotta finish this assignment
    // So I'm just going to work around it 
    // Typically we would pass in the transform but instead we are going to make a new transform
    // That has the correct values
    */
    
    glm::mat4 world = glm::mat4(1.0f);
    world = glm::translate(world, this->GetWorldPosition());
    
    // Apply rotation (assuming the billboard should always face the camera)
    world *= glm::mat4_cast(glm::quat(glm::radians(GetWorldRotation()))); 
    
    world = glm::scale(world, this->GetWorldScale());
    

    glm::mat4 MVP = proj * view * world;

    // Use shader program.
    if(m_pProgram == nullptr){
        std::cout << "Shader not added to DebugCube!" << std::endl;
        return;
    }

    m_pProgram->Bind();

    m_pProgram->SetUniform("MVP", MVP);

    m_pProgram->SetUniform("color", getColor());

    if(m_pTexture){
        m_pTexture->Bind(0);
        m_pProgram->SetUniform("tex", 0);
    }
    
    // Used for tiling
    m_pProgram->SetUniform("uScale", this->GetWorldScale().z);
    m_pProgram->SetUniform("vScale", this->GetWorldScale().x);

    m_pDecl->Bind();

    glDrawElements(GL_TRIANGLES, cubeIndices.size(), GL_UNSIGNED_SHORT, nullptr);

}
void BillBoard::SetTexture(const std::string& p_sTexturePath){
    m_pTexture = wolf::TextureManager::CreateTexture(p_sTexturePath);
    
    m_pTexture->SetFilterMode(wolf::Texture::FM_Linear, wolf::Texture::FM_LinearMipmap);
    m_pTexture->SetWrapMode(wolf::Texture::WM_Repeat, wolf::Texture::WM_Repeat);
}
void BillBoard::SetShader(const std::string& p_sVshPath, const std::string& p_sFshPath){
    m_pProgram = wolf::ProgramManager::CreateProgram(p_sVshPath, p_sFshPath);
}
AABB& BillBoard::CalculateVolume() {
    // Comments are all stolen from debug cube
    // For now made is the BillBoard is Huge so we don't need to calculate and It will always be in the frustum
    // Hopefully


    /* ERROR IN GetWorldTransform somewhere but I gotta finish this assignment
    // So I'm just going to work around it 
    // Typically we would pass in the transform but instead we are going to make a new transform
    // That has the correct values
    */
    //glm::mat4 transform = glm::mat4(1);
    //transform = glm::translate(transform, this->GetWorldPosition()) * glm::scale(transform, this->GetWorldScale());
    // Will transform to the center value so we want to set our trasnfor to that
    //m_bounds = AABB(transform);
    m_bounds = AABB(glm::vec3(0, 0, 0), 1000);
    return m_bounds;
}
void BillBoard::UpdateVerts(){

}
void BillBoard::FaceCamera(){
    // Get the view matrix from the active camera
    glm::mat4 viewMatrix = Scene::Instance().GetActiveCamera()->getViewMatrix();

    // Extracts the rotation matrix
    glm::mat3 rotationMatrix = glm::mat3(viewMatrix);

    
    glm::quat billboardRotation = glm::quat_cast(glm::transpose(rotationMatrix)); // Invert to face camera

    // Conver the quaternion into Euler angles 
    glm::vec3 euler = glm::degrees(glm::eulerAngles(billboardRotation));

    this->SetLocalRotation(euler);

}