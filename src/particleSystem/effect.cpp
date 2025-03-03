#include "effect.h"


Effect::Effect(){
    // Creates an effect with certain paramets I think
    SetVolume(CalculateVolume());
}
Effect::~Effect(){
    //Cleans out all the emitters
    m_pEmitters.clear();
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
void Effect::Render(glm::mat4 p_View, glm::mat4 p_Proj){
    for(Emitter* e: m_pEmitters){
        e->Render(p_View, p_Proj);
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

    //m_bounds = AABB(glm::vec3(0, 0, 0), 1000);
    return AABB(glm::vec3(0, 0, 0), 1000);
}