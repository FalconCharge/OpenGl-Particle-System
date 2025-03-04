#pragma once
#include "affector.h"

// Would be nice to add accerlation into this

class RotateAffector : public Affector{
    public:
        RotateAffector(const float rotationFactor) : m_rotation(rotationFactor) {}

        ~RotateAffector() {}


        void Apply(Particle* particle, float p_fDeltaTime){
            PointBB* point = static_cast<PointBB*>(particle);

            float scale = point->GetRotation();
            scale += m_rotation * p_fDeltaTime;

            point->SetRotation(scale);
        }

    private:
        float m_rotation;

};