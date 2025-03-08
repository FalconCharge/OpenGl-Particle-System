#pragma once
#include "affector.h"

// Would be nice to add accerlation into this

class GravityAffector : public Affector{
    public:
        GravityAffector(const float strenght) : m_gravity(strenght) {}

        ~GravityAffector() {}


        void Apply(Particle* particle, float p_fDeltaTime){
            PointBB* point = static_cast<PointBB*>(particle);

            glm::vec3 velocity = point->GetVelocity();
            velocity.y -= m_gravity * p_fDeltaTime;
            point->SetVelocity(velocity);

        }

    private:
        float m_gravity;

};