#pragma once
#include "emitter.h"


/*
// Derived from Emitter the Box emitter spawn particles randomly with in a box of a certain size
*/
class BoxEmitter : public Emitter{
    public:
        // Setter
        void SetBoxSize(glm::vec3 size){m_size = size;}
        
        void SetRandomPosition(Particle* p) override{
            PointBB* point = static_cast<PointBB*>(p);
            
            glm::vec3 pos = this->GetWorldPosition();
            GeneratePosition(pos.x, pos.y, pos.z);
            point->SetPosition(pos);
        }

    private:
        void GeneratePosition(float &x, float &y, float &z){
            // Generate random position inside the box
            float offsetX = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * m_size.x;
            float offsetY = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * m_size.y;
            float offsetZ = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * m_size.z;
            
            x += offsetX;
            y += offsetY;
            z += offsetZ;
        }
    
        glm::vec3 m_size = glm::vec3(1);
};