#pragma once
#include "affector.h"

// Would be nice to add accerlation into this

class ScaleAffector : public Affector{
    public:
        ScaleAffector(const float scaleFactor) : m_scale(scaleFactor) {}

        ~ScaleAffector() {}


        void Apply(Particle* particle, float p_fDeltaTime){
            PointBB* point = static_cast<PointBB*>(particle);

            float scale = point->GetScale();
            scale += m_scale * p_fDeltaTime;

            point->SetScale(scale);
        }

    private:
        float m_scale;

};