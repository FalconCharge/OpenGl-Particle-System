#include "effect.h"


#include <cstdlib>  // For rand()
#include <ctime>    // For time()

Effect::Effect(){
    // Creates an effect with certain paramets I think
    SetVolume(CalculateVolume());
    //glEnable(GL_DYNAMIC_DRAW); // Use when flushing the VB
    SetWorldPosition(glm::vec3(0));

    SetupRendering();
}
Effect::~Effect() {
    // Clean up all emitters
    for (Emitter* e : m_pEmitters) {
        if (e) {
            delete e;
        }
    }
    m_pEmitters.clear();

    // Clean up the vertex buffer
    if (m_pVB) {
        wolf::BufferManager::DestroyBuffer(m_pVB);
        m_pVB = nullptr;
    }

    // Clean up the vertex declaration
    if (m_pDecl) {
        delete m_pDecl;
        m_pDecl = nullptr;
    }
    if(m_pTexture){
        wolf::TextureManager::DestroyTexture(m_pTexture);
        m_pTexture = nullptr;
    }

    std::cout << "Effect destructor finished." << std::endl;
}

// Runs the the Emitters and Calls Update
void Effect::Update(float p_fDelta){
    for(Emitter* e: m_pEmitters){
        e->Update(p_fDelta);
    }
}
void Effect::Init(){
    for(Emitter* e: m_pEmitters){
        e->Init();
    }
}
void Effect::Play(){
    for(Emitter* e: m_pEmitters){
        e->Play();
    }
}
void Effect::Stop(){
    for(Emitter* e: m_pEmitters){
        e->Stop();
    }
}
void Effect::Pause(){
    for(Emitter* e: m_pEmitters){
        e->Pause();
    }
}
void Effect::AddEmitter(Emitter* e){
    m_pEmitters.push_back(e);
    this->AddChild(e);

}
AABB& Effect::CalculateVolume() {
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
void Effect::Render(glm::mat4 p_view, glm::mat4 p_Proj){
    // Temps
    std::vector<Point> batchVerts;
    wolf::Material* currentMat = nullptr;

    // Loop through Emitters getting their matterials and rendering em I think It's wrong RN BTW 
    for(Emitter* e: m_pEmitters){
        wolf::Material* emitterMat = e->GetMaterial();

        std::vector<Point> emitterVerts;
        e->GetVertexData(emitterVerts);

        if(currentMat == nullptr){
            currentMat = emitterMat;
        }
        if(emitterMat != currentMat){
            if(!batchVerts.empty()){
                // Setting the Project and view matrixs for all material
                currentMat->SetUniform("Proj", p_Proj);
                currentMat->SetUniform("View", p_view);

                FlushVB(currentMat, batchVerts);
                batchVerts.clear();
            }
            currentMat = emitterMat;
        }
        batchVerts.insert(batchVerts.end(), emitterVerts.begin(), emitterVerts.end());
    }
    // Flush any remaining Verts
    if(!batchVerts.empty() && currentMat != nullptr){
        currentMat->SetUniform("Proj", p_Proj);
        currentMat->SetUniform("View", p_view);

        FlushVB(currentMat, batchVerts);
    }
}
void Effect::FlushVB(wolf::Material* currMaterial, const std::vector<Point>& vertices){
    //glEnable(GL_DEPTH_TEST);
    //glDepthMask(GL_FALSE);

    if(m_pVB == nullptr){
        std::cout << "[ERROR] m_pVB is null | Location: Effect class" << std::endl;
        return;
    }

    // Print vertex data before writing to the buffer
    /*
    std::cout << "[Vertex Data] Begin\n";
    for (const Point& vertex : vertices) {
        std::cout << "Position: (" << vertex.x << ", " << vertex.y << ", " << vertex.z << "), "
                  << "Size: " << vertex.w << ", "
                  << "Color: (" << vertex.r << ", " << vertex.g << ", " << vertex.b << "), "
                  << "Alpha: " << vertex.a << ", "
                  << "Rotation: " << vertex.rotation << std::endl;
    }
    std::cout << "[Vertex Data] End\n";*/

    // Write the vertex data to the vertex buffer
    m_pVB->Write(vertices.data(), vertices.size() * sizeof(Point));

    currMaterial->Apply();

    m_pDecl->Bind();

    glDrawArrays(GL_POINTS, 0, vertices.size());

    //glDepthMask(GL_TRUE);
}

void Effect::SetupRendering(){
    // Shader Is set in Emitters
    m_pVB = wolf::BufferManager::CreateVertexBuffer(100000);    // Max size 100k?

    m_pDecl = new wolf::VertexDeclaration();

    m_pDecl->Begin();
    m_pDecl->AppendAttribute(wolf::AT_Position, 4, wolf::CT_Float);     // Pos attr + scale.w
    m_pDecl->AppendAttribute(wolf::AT_Color, 4, wolf::CT_Float);    // Rotation attr
    m_pDecl->AppendAttribute(wolf::AT_TexCoord1, 1, wolf::CT_Float);    // Rotation attr

    m_pDecl->SetVertexBuffer(m_pVB);
    m_pDecl->End();
}