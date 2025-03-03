#include "emitter.h"

#include <random>

void Emitter::Init(){
    PointBB* pParticles = new PointBB[NUM_OF_PARTICLES_IN_POOL];
    for(int i = 0; i < NUM_OF_PARTICLES_IN_POOL; i++){
        this->AddToPool(&pParticles[i]);
    }
    std::cout << "Init Emitter" << std::endl;
}
void Emitter::AddToPool(Particle* p){
    p->prev = 0;
    p->next = m_pFreeList;
    if(m_pFreeList){
        m_pFreeList->prev = p;
    }
    m_pFreeList = p;
}
Particle* Emitter::GetFreeParticle() {
    Particle* pRet = m_pFreeList;
    if (pRet) {
        m_pFreeList = m_pFreeList->next;
        if (m_pFreeList) {
            m_pFreeList->prev = nullptr;
        }
        pRet->prev = nullptr;
        pRet->next = nullptr;
    }
    return pRet;
}
void Emitter::SpawnParticle(){
    Particle* p = GetFreeParticle();

    // If not free Particle, Kill one from the active list
    if(!p){
        if(m_pActiveList){
            Particle* pToKill = m_pActiveList;
            ParticleKilled(pToKill);
            // Get the free Particle now
            p = GetFreeParticle();
        }
    }

    if(p){
        AddToActive(p);
        std::cout << "Spawning Particle" << std::endl;
    }else{
        std::cout << "[ERROR] no particles to spawn" << std::endl;
    }

}
void Emitter::ParticleKilled(Particle* p){
    RemoveFromActive(p);
    AddToFree(p);

    std::cout << "Killed Particle" << std::endl;
}
void Emitter::SetMode(const std::string& p_sMode) {
    if (p_sMode == "CONTINUOUS" || p_sMode == "BURST") {
        m_sMode = p_sMode;
    } else {
        m_sMode = "CONTINUOUS";
    }
}
void Emitter::Update(float p_fDelta){
    if(m_sMode == "CONTINUOUS"){
        int birth_Rate = 0;
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
}
void Emitter::Render(glm::mat4 p_view, glm::mat4 p_Proj) {
    // Render all active particles.
    // I'm guessing I want to create a huge VB with all the data to render?

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
// A helper that adds the particle back to the free list.
void Emitter::AddToFree(Particle* p) {
    AddToPool(p);
}

void Emitter::CalcBurstTime() {
    // Calculate and update burst timing if using burst mode.
}
