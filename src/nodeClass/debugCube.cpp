#include "debugcube.h"


DebugCube::DebugCube(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation) : m_bounds(glm::mat4(1)){
    SetLocalPosition(position);
    SetLocalScale(scale);
    SetLocalRotation(rotation);
    SetName("DebugCube");

}
DebugCube::~DebugCube(){
    delete m_pDecl;
    wolf::BufferManager::DestroyBuffer(m_pIB);
    wolf::BufferManager::DestroyBuffer(m_pVB);
    wolf::ProgramManager::DestroyProgram(m_pProgram);
    wolf::TextureManager::DestroyTexture(m_pTexture);

    normalVertices.clear();
    cubeIndices.clear();
}

void DebugCube::Init(){
    SetDirty(true);
    //std::cout << "Initlizing: " << GetName() << std::endl;
    renderBB();
    if(m_pProgram == nullptr){
        m_pProgram = wolf::ProgramManager::CreateProgram("Data/Shader/DebugCube.vsh", "Data/Shader/DebugCube.fsh");
    }
    // Cube vertex data with texture coordinates and normals CHATGPT GENERATED
    // I'm guessing a better option like creating serperate vectors for pos, uvs, normals just to make it more clean (add them together)
    normalVertices = {
        // Positions              // Texture Coords   // Smoothed Normals
        // Front face (z = 0.5)
        { -0.5f, -0.5f,  0.5f,     0.0f, 0.0f,    -0.577f, -0.577f,  0.577f }, // Bottom-left
        {  0.5f, -0.5f,  0.5f,     1.0f, 0.0f,     0.577f, -0.577f,  0.577f }, // Bottom-right
        {  0.5f,  0.5f,  0.5f,     1.0f, 1.0f,     0.577f,  0.577f,  0.577f }, // Top-right
        { -0.5f,  0.5f,  0.5f,     0.0f, 1.0f,    -0.577f,  0.577f,  0.577f }, // Top-left

        // Back face (z = -0.5)
        { -0.5f, -0.5f, -0.5f,     0.0f, 0.0f,    -0.577f, -0.577f, -0.577f }, // Bottom-left
        {  0.5f, -0.5f, -0.5f,     1.0f, 0.0f,     0.577f, -0.577f, -0.577f }, // Bottom-right
        {  0.5f,  0.5f, -0.5f,     1.0f, 1.0f,     0.577f,  0.577f, -0.577f }, // Top-right
        { -0.5f,  0.5f, -0.5f,     0.0f, 1.0f,    -0.577f,  0.577f, -0.577f }, // Top-left

        // Bottom face (y = -0.5)
        { -0.5f, -0.5f, -0.5f,     0.0f, 0.0f,    -0.577f, -0.577f, -0.577f }, // Back-left
        {  0.5f, -0.5f, -0.5f,     1.0f, 0.0f,     0.577f, -0.577f, -0.577f }, // Back-right
        {  0.5f, -0.5f,  0.5f,     1.0f, 1.0f,     0.577f, -0.577f,  0.577f }, // Front-right
        { -0.5f, -0.5f,  0.5f,     0.0f, 1.0f,    -0.577f, -0.577f,  0.577f }, // Front-left

        // Top face (y = 0.5)
        { -0.5f,  0.5f, -0.5f,     0.0f, 0.0f,    -0.577f,  0.577f, -0.577f }, // Back-left
        {  0.5f,  0.5f, -0.5f,     1.0f, 0.0f,     0.577f,  0.577f, -0.577f }, // Back-right
        {  0.5f,  0.5f,  0.5f,     1.0f, 1.0f,     0.577f,  0.577f,  0.577f }, // Front-right
        { -0.5f,  0.5f,  0.5f,     0.0f, 1.0f,    -0.577f,  0.577f,  0.577f }, // Front-left

        // Left face (x = -0.5)
        { -0.5f, -0.5f, -0.5f,     0.0f, 0.0f,    -0.577f, -0.577f, -0.577f }, // Back-bottom
        { -0.5f,  0.5f, -0.5f,     1.0f, 0.0f,    -0.577f,  0.577f, -0.577f }, // Back-top
        { -0.5f,  0.5f,  0.5f,     1.0f, 1.0f,    -0.577f,  0.577f,  0.577f }, // Front-top
        { -0.5f, -0.5f,  0.5f,     0.0f, 1.0f,    -0.577f, -0.577f,  0.577f }, // Front-bottom

        // Right face (x = 0.5)
        {  0.5f, -0.5f, -0.5f,     0.0f, 0.0f,     0.577f, -0.577f, -0.577f }, // Back-bottom
        {  0.5f,  0.5f, -0.5f,     1.0f, 0.0f,     0.577f,  0.577f, -0.577f }, // Back-top
        {  0.5f,  0.5f,  0.5f,     1.0f, 1.0f,     0.577f,  0.577f,  0.577f }, // Front-top
        {  0.5f, -0.5f,  0.5f,     0.0f, 1.0f,     0.577f, -0.577f,  0.577f }  // Front-bottom
    };

    cubeIndices = {
        // Front face
        0, 1, 2,  // Triangle 1
        0, 2, 3,  // Triangle 2

        // Back face
        4, 6, 5,    // Triangle 1
        4, 7, 6,    // Triangle 2

        // Left face
        8, 9, 10, // Triangle 1
        8, 10, 11, // Triangle 2

        // Right face
        12, 14, 13, // Triangle 1
        12, 15, 14, // Triangle 2

        // Top face
        16, 18, 17, // Triangle 1
        16, 19, 18, // Triangle 2

        // Bottom face
        20, 21, 22, // Triangle 1
        20, 22, 23  // Triangle 2
    };

    //Create VB & IB & VertexDeclation

    m_pVB = wolf::BufferManager::CreateVertexBuffer(normalVertices.data(), normalVertices.size() * sizeof(CubeVertex));
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
void DebugCube::Render(glm::mat4 proj, glm::mat4 view){
    Node::Render(proj, view);

    /* 
    // ERROR IN GetWorldTransform somewhere but I gotta finish this assignment
    // So I'm just going to work around it 
    // Typically we would pass in the transform but instead we are going to make a new transform
    // That has the correct values
    */

    glEnable(GL_DEPTH_TEST);
    //glDisable(GL_BLEND);
    //glDepthMask(GL_TRUE);  // Enable depth writing
    //glEnable(GL_DEPTH_TEST);


    
    glm::mat4 world = glm::mat4(1);
    world = glm::translate(world, this->GetWorldPosition()) * glm::scale(world, this->GetWorldScale());

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

    m_pProgram->SetUniform("uScale", this->GetWorldScale().z);
    m_pProgram->SetUniform("vScale", this->GetWorldScale().x);

    m_pDecl->Bind();

    glDrawElements(GL_TRIANGLES, cubeIndices.size(), GL_UNSIGNED_SHORT, nullptr);

    if(m_renderBB){
        GetVolume().Render(view, proj);
    }

        glEnable(GL_DEPTH_TEST);


}
void DebugCube::Update(float fDelta){
    Node::Update(fDelta);

    // We don't make any updates on this cube
    return;
}

AABB& DebugCube::CalculateVolume() {
    /* ERROR IN GetWorldTransform somewhere but I gotta finish this assignment
    // So I'm just going to work around it 
    // Typically we would pass in the transform but instead we are going to make a new transform
    // That has the correct values
    */
    glm::mat4 transform = glm::mat4(1);
    transform = glm::translate(transform, this->GetWorldPosition()) * glm::scale(transform, this->GetWorldScale());
    // Will transform to the center value so we want to set our trasnfor to that
    m_bounds = AABB(transform);
    return m_bounds;
}

void DebugCube::SetShader(const std::string& vshPath, const std::string& fshPath){
    m_pProgram = wolf::ProgramManager::CreateProgram(vshPath, fshPath);
    
}
void DebugCube::SetTexture(const std::string& path){
    m_pTexture = wolf::TextureManager::CreateTexture(path);
    m_pTexture->SetFilterMode(wolf::Texture::FM_Linear, wolf::Texture::FM_LinearMipmap);
    m_pTexture->SetWrapMode(wolf::Texture::WM_Repeat, wolf::Texture::WM_Repeat);
}
