#include "Quad.h"
//Constructor
Quad::Quad(){
    Init();
}

// Deconstructor
Quad::~Quad() {
    delete m_pDecl;
	wolf::BufferManager::DestroyBuffer(m_VBquad);
    wolf::TextureManager::DestroyTexture(m_texture);
    vertices.clear();
}  
//Creates a Shader and generates the verts
void Quad::Init() {
    m_pProgram = wolf::ProgramManager::CreateProgram("src/textRendering/Data/Shaders/Quad.vsh", "src/textRendering/Data/Shaders/Quad.fsh");
    generateVertices();
}
//used to render the Quad with it's own draw call
void Quad::render(int width, int height) {
    
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

    // Bind the texture
    if(m_texture != nullptr){
        m_texture->Bind();
        m_pProgram->SetUniform("tex", 0);
    }

    // Bind the vertex declaration
    m_pDecl->Bind();

    // Draw the quad (we use 6 indices for a full quad)
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
//Sets the texture coordinates
void Quad::SetTextureCoordinates(GLfloat u1, GLfloat v1, GLfloat u2, GLfloat v2){
    m_u1 = u1;
    m_u2 = u2;
    m_v1 = v1;
    m_v2 = v2;
    generateVertices();
}
// First used when setting up the quad for a base
void Quad::generateVertices() {
    // Create the vertices for the quad
    
    vertices = {
        {  0.0f, 0.0f, m_u1, m_v2 }, // Bottom-left
        {  1.0f, 0.0f, m_u2, m_v2 }, // Bottom-right
        {  1.0f, 1.0f, m_u2, m_v1 }, // Top-right

        {  0.0f, 0.0f, m_u1, m_v2 }, // Bottom-left
        {  1.0f, 1.0f, m_u2, m_v1 }, // Top-right
        {  0.0f, 1.0f, m_u1, m_v1 }  // Top-left
    };

    //Delete the old VB and Declartion
    if(m_VBquad){
        wolf::BufferManager::DestroyBuffer(m_VBquad);
        m_VBquad = nullptr;
    }
    if(m_pDecl){
        delete m_pDecl;
        m_pDecl = nullptr;
    }

    //Create a VB & declarion with the new data
    m_VBquad = wolf::BufferManager::CreateVertexBuffer(vertices.data(), vertices.size() * sizeof(Vertex));
    m_pDecl = new wolf::VertexDeclaration();

    m_pDecl->Begin();
    m_pDecl->AppendAttribute(wolf::AT_Position, 2, wolf::CT_Float);
    m_pDecl->AppendAttribute(wolf::AT_TexCoord1, 2, wolf::CT_Float);
    m_pDecl->SetVertexBuffer(m_VBquad);
    m_pDecl->End();

    m_isDirty = true;
    updateVertices();
}
//Used to update the values of a vertices when a change is made to the size, scale, position
void Quad::updateVertices() {
    glm::mat4 transform = glm::mat4(1.0f);
    // Calculate transformation based on position, scale, and rotation
    transform = glm::translate(glm::mat4(1.0f), glm::vec3(m_vPosition, 0.0f)) *
                glm::rotate(transform, glm::radians(m_fRotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
                glm::scale(transform, glm::vec3(m_vScale, 1.0f));

    // Update the vertex positions
    std::vector<Vertex> transformedVertices(vertices.size());
    for (size_t i = 0; i < vertices.size(); ++i) {
        glm::vec4 pos = transform * glm::vec4(vertices[i].x, vertices[i].y, 0.0f, 1.0f);
        transformedVertices[i] = {
            pos.x, pos.y,
            vertices[i].u, vertices[i].v
        };
    }

    //Create a VB & declarion with the new data
    m_VBquad = wolf::BufferManager::CreateVertexBuffer(transformedVertices.data(), transformedVertices.size() * sizeof(Vertex));
    m_pDecl = new wolf::VertexDeclaration();

    m_pDecl->Begin();
    m_pDecl->AppendAttribute(wolf::AT_Position, 2, wolf::CT_Float);
    m_pDecl->AppendAttribute(wolf::AT_TexCoord1, 2, wolf::CT_Float);
    m_pDecl->SetVertexBuffer(m_VBquad);
    m_pDecl->End(); 


    //Set them as the new verts
    m_newVerts = transformedVertices;

    m_isDirty = true;
}
//Sets the scale of the quad
void Quad::setScale(glm::vec2 scale){
    m_vScale = scale;
    updateVertices();
}
//Returns the updated vertices
std::vector<Vertex> Quad::GetVertices(){
    m_isDirty = false;
    return m_newVerts;
    
}



