#include "effect.h"


#include <cstdlib>  // For rand()
#include <ctime>    // For time()

Effect::Effect(){
    
    SetVolume(CalculateVolume());

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

    std::cout << "Effect Destoryed." << std::endl;
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

void Effect::GetVertexData(){
    m_pPoints.clear();

    std::vector<PointBB*> pointBBs;
    pointBBs.clear();

    for(Emitter* e : m_pEmitters){
        e->GetPointBB(pointBBs);
    }

    // Sort them into a proper order
    m_pPoints = SortPoints(pointBBs);


}
std::vector<Point> Effect::SortPoints(std::vector<PointBB*> pointBBS) {
    // Sort PointBBs by camera distance (descending order, farthest first)
    std::sort(pointBBS.begin(), pointBBS.end(), [](PointBB* a, PointBB* b) {
        return a->GetCameraDistance() < b->GetCameraDistance();  // Sort by farthest first
    });

    // Convert sorted PointBBs into Point structs
    std::vector<Point> sortedPoints;
    sortedPoints.reserve(pointBBS.size());

    for (PointBB* bb : pointBBS) {
        Point p;
        p.x = bb->GetPosition().x;
        p.y = bb->GetPosition().y;
        p.z = bb->GetPosition().z;
        p.w = bb->GetSize(); 

        p.r = bb->GetColor().r;
        p.g = bb->GetColor().g;
        p.b = bb->GetColor().b;
        p.a = bb->GetFade();

        p.rotation = bb->GetRotation();

        sortedPoints.push_back(p);
    }

    return sortedPoints;
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

    // Gather and sort points
    GetVertexData();  // m_pPoints is now filled with sorted points

    // For this example, assume all points use the same material.
    // (If they don’t, you need additional data to know which point belongs to which material.)
    if(m_pPoints.empty() || m_pEmitters.empty()) {
        return;
    }
    
    // Get material from first emitter (or choose your desired one)
    wolf::Material* mat = m_pEmitters[0]->GetMaterial();
    mat->SetUniform("Proj", p_Proj);
    mat->SetUniform("View", p_view);
    
    // Render the sorted points
    FlushVB(mat, m_pPoints);

    /*
    // Temps
    std::vector<Point> batchVerts;
    wolf::Material* currentMat = nullptr;

    GetVertexData();

    std::unordered_map<wolf::Material*, std::vector<Point>> materialBatches;

    for(Emitter* e : m_pEmitters){
        wolf::Material* emitterMat = e->GetMaterial();
        materialBatches[emitterMat].insert(
            materialBatches[emitterMat].end(),
            batchVerts.begin(),
            batchVerts.end()
        );
    }

    // Render each batch by material
    for (auto& [mat, verts] : materialBatches) {
        if (!verts.empty()) {
            mat->SetUniform("Proj", p_Proj);
            mat->SetUniform("View", p_view);
            FlushVB(mat, verts);
        }
    }*/

    /*
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
    }*/
}
void Effect::FlushVB(wolf::Material* currMaterial, std::vector<Point>& vertices){
    if(m_pVB == nullptr){
        std::cout << "[ERROR] m_pVB is null | Location: Effect class" << std::endl;
        return;
    }

    // Write the vertex data to the vertex buffer
    m_pVB->Update(vertices.data(), vertices.size() * sizeof(Point));

    currMaterial->Apply();

    m_pDecl->Bind();

    // Save the current depth write state
    GLboolean depthWriteEnabled;
    glGetBooleanv(GL_DEPTH_WRITEMASK, &depthWriteEnabled);

    // This is changing too much IMO I think it's nessarcy though
    // ALso it should be in material But that don't wanna work some reason
    // And I don't wanna try to fix it when I have a fix here
    if (depthWriteEnabled) {
        glDepthMask(GL_FALSE);
    }

    // SHould be moved to Material
    glDrawArrays(GL_POINTS, 0, vertices.size());

    // Gotta change it back for the ground
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
}

void Effect::SetupRendering(){
    // Sets up the VB which will collect all the points data
    m_pVB = wolf::BufferManager::CreateVertexBuffer(1000000);    // Max size 100k?

    m_pDecl = new wolf::VertexDeclaration();

    m_pDecl->Begin();
    m_pDecl->AppendAttribute(wolf::AT_Position, 4, wolf::CT_Float);     // Pos attr + scale.w
    m_pDecl->AppendAttribute(wolf::AT_Color, 4, wolf::CT_Float);    // Rotation attr
    m_pDecl->AppendAttribute(wolf::AT_TexCoord1, 1, wolf::CT_Float);    // Rotation attr

    m_pDecl->SetVertexBuffer(m_pVB);
    m_pDecl->End();
}