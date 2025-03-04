#pragma once
#include "emitter.h"


/*
// Derived from Emitter the Box emitter spawn particles randomly with in a Sphere of a certain radius
*/
class BoxEmitter : public Emitter{
    public:
        // Setter
        void SetRadius(float radius){m_radius = radius;}
        void SetRandomPosition(Particle* p) override {
            PointBB* point = static_cast<PointBB*>(p);
    
            float x, y, z;
            GeneratePosition(x, y, z);
            point->SetPosition(glm::vec3(x, y, z));
        }

    private:
        void GeneratePosition(float &x, float &y, float &z) {
            // Generate a random point inside a sphere using spherical coordinates
            float theta = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.0f * PI;    // Random angle [0, 2π]
            float phi = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * PI;             // Random angle [0, π]
            float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * m_radius;        // Random radius [0, m_radius]

            // Convert spherical coordinates to Cartesian coordinates
            x = r * sin(phi) * cos(theta);
            y = r * sin(phi) * sin(theta);
            z = r * cos(phi);
        }
    
        float m_radius = 1;
};