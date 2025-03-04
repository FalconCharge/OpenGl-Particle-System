#pragma once
#include "affector.h"

// Would be nice to add accerlation into this

class GravityAffector : public Affector{
    public:
        GravityAffector(const float gravity) : m_gravity(gravity) {}

        ~GravityAffector() {}


        void Apply(Particle* particle, float p_fDeltaTime){
            PointBB* point = static_cast<PointBB*>(particle);

            glm::vec3 velocity = point->GetVelocity();
            velocity.y += m_gravity * p_fDeltaTime;

            glm::vec3 pos = point->GetPosition();

            pos -= velocity * p_fDeltaTime;

            point->SetPosition(pos);
            point->SetVelocity(velocity);

        }

    private:
        float m_gravity;

};