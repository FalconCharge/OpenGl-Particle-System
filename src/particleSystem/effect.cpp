#include "effect.h"

#include <chrono>
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

    //std::cout << "Effect Destoryed." << std::endl;
}

// Runs the the Emitters and Calls Update
void Effect::Update(float p_fDelta){
    for(Emitter* e: m_pEmitters){
        e->Update(p_fDelta);
    }
    GetVertexData();  // m_pPoints is now filled with sorted points

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

    //RadixSort(pointBBs);


    // Sort them into a proper order
    m_pPoints = SortPoints(pointBBs);


}
std::vector<Point> Effect::SortPoints(std::vector<PointBB*> pointBBS) {

    std::sort(pointBBS.begin(), pointBBS.end(), [](PointBB* a, PointBB* b) {
        return a->GetCameraDistance() < b->GetCameraDistance();
    });

    // Convert sorted PointBBs into Point structs
    std::vector<Point> sortedPoints;
    sortedPoints.reserve(pointBBS.size());

    for (PointBB* bb : pointBBS) {

        sortedPoints.emplace_back(bb->GetPosition(), bb->GetColor(), bb->GetSize(), bb->GetFade(), bb->GetRotation());
    }

    return sortedPoints;
}
void Effect::RadixSort(std::vector<PointBB*>& pointBBS) {
    // Ensure there are no invalid pointers
    for (const auto& bb : pointBBS) {
        if (bb == nullptr) {
            std::cerr << "Invalid PointBB pointer!" << std::endl;
            return;  // If there's an invalid pointer, exit the function
        }
    }

    // Find the max distance for normalization
    GLfloat maxDist = 0.0f;
    for (const auto& bb : pointBBS) {
        maxDist = std::max(maxDist, std::abs(bb->GetCameraDistance()));  // Use abs to handle negative distances
    }

    // Perform Radix sort for the distances
    for (GLfloat exp = 1.0f; maxDist / exp > 1.0f; exp *= 10) {
        CountSort(pointBBS, exp);
    }
}

void Effect::CountSort(std::vector<PointBB*>& pointBBS, GLfloat exp) {
    std::vector<PointBB*> output(pointBBS.size());
    int count[10] = {0};  // For storing counts of digits (0-9)

    // Count occurrences of each digit
    for (auto& bb : pointBBS) {
        int digit = static_cast<int>(std::abs(bb->GetCameraDistance()) / exp) % 10;  // Use abs for negative distances
        count[digit]++;
    }

    // Cumulative count
    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }

    // Build output array (sorted by current digit)
    for (int i = pointBBS.size() - 1; i >= 0; i--) {
        int digit = static_cast<int>(std::abs(pointBBS[i]->GetCameraDistance()) / exp) % 10;
        output[count[digit] - 1] = pointBBS[i];
        count[digit]--;
    }

    // Clear the original vector and copy the sorted data back
    pointBBS.clear();
    pointBBS.insert(pointBBS.end(), output.begin(), output.end());
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
    // Can only deal with 1 Material So Not multi Textures emitters

    // Gather and sort points
    //GetVertexData();  // m_pPoints is now filled with sorted points
    

    // For this example, assume all points use the same material.
    // (If they don’t, you need additional data to know which point belongs to which material.)
    if(m_pPoints.empty() || m_pEmitters.empty()) {
        return;
    }
    
    // Get material from first emitter (or choose your desired one)
    wolf::Material* mat = m_pEmitters[0]->GetMaterial();
    m_pEmitters[0]->GetTexture();
    mat->SetTexture("texture1", m_pEmitters[0]->GetTexture());
    mat->SetUniform("Proj", p_Proj);
    mat->SetUniform("View", p_view);

    
    // Render the sorted points
    FlushVB(mat, m_pPoints);
}
void Effect::FlushVB(wolf::Material* currMaterial, std::vector<Point>& vertices){
    glDepthMask(GL_FALSE);
    if(m_pVB == nullptr){
        std::cout << "[ERROR] m_pVB is null | Location: Effect class" << std::endl;
        return;
    }

    // Write the vertex data to the vertex buffer
    m_pVB->Update(vertices.data(), vertices.size() * sizeof(Point));

    currMaterial->Apply();

    m_pDecl->Bind();


    // This is changing too much IMO I think it's nessarcy though
    // ALso it should be in material But that don't wanna work some reason
    // And I don't wanna try to fix it when I have a fix here

    // SHould be moved to Material
    glDrawArrays(GL_POINTS, 0, vertices.size());

    glDepthMask(GL_TRUE);

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