#include "emitter.h"
#include "affector.h"

#include <random>
Emitter::Emitter(){
    
}

void Emitter::Init(){
    
    PointBB* pParticles = new PointBB[NUM_OF_PARTICLES_IN_POOL];
    for(int i = 0; i < NUM_OF_PARTICLES_IN_POOL; i++){
        this->AddToPool(&pParticles[i]);
    }


    // Create the Material for the emitter
    m_pMaterial = wolf::MaterialManager::CreateMaterial("Emitter Material");

    // Enable Depth test and blending and set the shader
    m_pMaterial->SetDepthTest(true);
    m_pMaterial->SetBlend(true);
    //m_pMaterial->SetPoint(true); Enable the     glEnable(GL_PROGRAM_POINT_SIZE); currently set in driver
    m_pMaterial->SetProgram("Data/Shader/glPoint.vsh", "Data/Shader/default.fsh");

    m_pTexture = wolf::TextureManager::CreateTexture("Data/textures/particles/smoke_01.png");
    m_pMaterial->SetTexture("texture1", m_pTexture);
    m_pMaterial->SetUniform("color", this->getColor());

    m_pMaterial->SetUniform("size", 500);   // Sets the size of the Point should be moved into PointBB
    m_pMaterial->SetUniform("size", 1500);   // Sets the size of the Point should be moved into PointBB


    m_pMaterial->SetUniform("aRotation", 45);   // Just rotates the UVs so it's still a square


    // Static Materials (This should change)

    SetVolume(CalculateVolume());

    std::cout << "Init Emitter" << std::endl;
}
void Emitter::AddToPool(Particle* p) {
    p->prev = nullptr;                    // Set the previous pointer of the particle to nullptr (indicating it's at the start of the list)
    p->next = m_pFreeList;          // Point the particle's next to the current head of the free list (this is effectively inserting the particle at the start of the free list)

    m_pFreeList = p;                // Make the new particle the first particle in the free list
}

Particle* Emitter::GetFreeParticle() {
    if(m_pFreeList == nullptr){return nullptr;} //Return null if the list is empty

    Particle* pRet = m_pFreeList;
    
    m_pFreeList = m_pFreeList->next;    //was null
    if (m_pFreeList) {
        m_pFreeList->prev = nullptr;
    }
    pRet->prev = nullptr;
    pRet->next = nullptr;
    

    
    return pRet;
}
void Emitter::SpawnParticle(){
    Particle* p = GetFreeParticle();

    // If not free Particle, Kill one from the active list
    if(!p){
        if(m_pActiveList){
            Particle* tail = m_pActiveList;
            while(tail->next){
                tail = tail->next;
            }

            RecycleParticle(tail);
            // Get the free Particle now
            p = GetFreeParticle();
            std::cout << "Got free particle through recycling" << std::endl;
        }
    }

    if(p){
        // Get random positions to spawn the Point at
        SetRandomPosition(p);
        AddToActive(p);
    }else{
        std::cout << "[ERROR] no particles to spawn" << std::endl;
    }

}
void Emitter::ParticleKilled(Particle* p){
    RemoveFromActive(p);
    AddToFree(p);

    // Resets values that need to be reset for a new life
    PointBB* point = static_cast<PointBB*>(p);
    point->ResetPoint();

    //std::cout << "Killed Particle" << std::endl;
}
void Emitter::RecycleParticle(Particle* p) {
    if(p == nullptr) {
        std::cout << "[ERROR] Attempt to recycle a null particle!" << std::endl;
        return;
    }

    RemoveFromActive(p);
    
    // Reset particle's state for reuse
    PointBB* point = static_cast<PointBB*>(p);
    point->ResetPoint();

    // Add the particle back to the free list (the pool)
    AddToFree(p);
}

void Emitter::SetMode(const std::string& p_sMode) {
    if (p_sMode == "CONTINUOUS" || p_sMode == "BURST") {
        m_sMode = p_sMode;
    } else {
        m_sMode = "CONTINUOUS";
    }
}
void Emitter::Update(float p_fDelta){
    // Update active particles
    Particle* currentParticle = m_pActiveList;

    while (currentParticle != 0) {
        Particle* nextParticle = currentParticle->next;

        PointBB* point = static_cast<PointBB*>(currentParticle);
        // Update particle
        point->Update(p_fDelta);

        // If expired, remove it from active and add to free list
        if (point->IsExpired()) {
            ParticleKilled(currentParticle);
        }

        currentParticle = nextParticle;

    }

    


    if(m_sMode == "CONTINUOUS"){
        float birth_Rate = 0;
        if (m_bRandomBurstRate){
            // From the internet not tested
            int range = m_fBurstTimeMax - m_fBurstTimeMin + 1;
            int num = rand() % range + m_fBurstTimeMin;
            birth_Rate = num;
        }
        else{
            birth_Rate = m_birthRate;
        }

        m_toSpawn_accumulator += birth_Rate * p_fDelta;


        
        m_numSpawns = int(m_toSpawn_accumulator);
        m_toSpawn_accumulator -= m_numSpawns;

        for (int i = 0; i < m_numSpawns; i++) {
            SpawnParticle();
        }
        
    }
    ApplyAffectors(p_fDelta);
}

void Emitter::Play() {
    // Start or resume particle emission.
    std::cout << "Pressed Play!" << std::endl;
}

void Emitter::Stop() {
    // Stop emission and possibly clear active particles.
}

void Emitter::Pause() {
    // Pause emission.
}
// Adds a particle to the active list.
void Emitter::AddToActive(Particle* p) {
    p->prev = nullptr;
    p->next = m_pActiveList;
    if (m_pActiveList) {
        m_pActiveList->prev = p;
    }
    m_pActiveList = p;
}
// Removes a particle from the active list.
void Emitter::RemoveFromActive(Particle* p) {
    if (p->prev)
        p->prev->next = p->next;
    else
        m_pActiveList = p->next;  // p was the head.

    if (p->next)
        p->next->prev = p->prev;
    
    p->prev = nullptr;
    p->next = nullptr;
}
void Emitter::RemoveTail(){
    if(m_pActiveList == nullptr) return;

    if(m_pActiveList->next = nullptr){
        m_pActiveList = nullptr;
    }else{
        Particle* tail = m_pActiveList;
        while(tail->next){
            tail = tail->next;
        }
        if(tail->prev){
            tail->prev->next = nullptr;
        }
        tail->next = nullptr;
        tail->prev = nullptr;
    }
}





// A helper that adds the particle back to the free list.
void Emitter::AddToFree(Particle* p) {
    AddToPool(p);
}

void Emitter::CalcBurstTime() {
    // Calculate and update burst timing if using burst mode.
}
wolf::Material* Emitter::GetMaterial(){
    return m_pMaterial;
}
void Emitter::GetVertexData(std::vector<Point>& vertexData){
    Particle* currentParticle = m_pActiveList;

    while(currentParticle != nullptr){
        PointBB* point = static_cast<PointBB*>(currentParticle);

        Point pointVertex;
        pointVertex.x = point->GetPosition().x;
        pointVertex.y = point->GetPosition().y;
        pointVertex.z = point->GetPosition().z;
        pointVertex.w = point->GetScale();

        pointVertex.rotation = point->GetRotation();

        vertexData.push_back(pointVertex);

        currentParticle = currentParticle->next;

    }
    vertexData.insert(vertexData.end(), m_vertexData.begin(), m_vertexData.end());
}
void Emitter::ApplyAffectors(float p_fDelta){
    Particle* current = m_pActiveList;
    while(current){
        PointBB* point = static_cast<PointBB*>(current);
        for(auto& affector : m_affectors){
            affector->Apply(point, p_fDelta);
        }
        current = current->next;
    }
}
AABB& Emitter::CalculateVolume() {
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