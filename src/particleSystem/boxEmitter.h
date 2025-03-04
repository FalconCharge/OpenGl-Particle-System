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

            float x, y, z;
            GeneratePosition(x, y, z);
            point->SetPosition(glm::vec3(x, y, z));
        }

    private:
        void GeneratePosition(float &x, float &y, float &z){
            // Generate random position inside the box
            x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * m_size.x;
            y = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * m_size.y;
            z = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * m_size.z;
        }
    
        glm::vec3 m_size = glm::vec3(1);
};