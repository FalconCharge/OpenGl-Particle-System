#pragma once
#include "affector.h"

#include <string>
#include "../utility/Unility.h"


class TurbulenceAffector : public Affector{
    public:
        TurbulenceAffector(float strength) : m_strength(strength) {}

        ~TurbulenceAffector() {}

        void Apply(Particle* particle, float deltaTime) override {
            // Cast the particle to PointBB (or your particle class)
            PointBB* point = static_cast<PointBB*>(particle);
    
            // Generate random offsets for turbulence
            glm::vec3 randomOffset(
                RandomFloat(-1.0f, 1.0f),
                RandomFloat(-1.0f, 1.0f),
                RandomFloat(-1.0f, 1.0f)
            );
    
            // Scale the random offset by strength and deltaTime
            glm::vec3 turbulence = randomOffset * m_strength * deltaTime;
    
            // Apply turbulence to the particle's velocity or position
            point->SetVelocity(point->GetVelocity() + turbulence);
        }

    private:
        float m_strength;

};