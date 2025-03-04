#pragma once
#include "affector.h"

// Would be nice to add accerlation into this

class LifeTimeAffector : public Affector{
    public:
        LifeTimeAffector(const float maxLifeTime) : m_MaxLifeTime(maxLifeTime) {}

        ~LifeTimeAffector() {}


        void Apply(Particle* particle, float p_fDeltaTime){
            return;
            PointBB* point = static_cast<PointBB*>(particle);
            
            if(m_timeAlive >= m_MaxLifeTime){
                point->SetIsExpired(true);
            }else{
                m_timeAlive += p_fDeltaTime;
            }


        }

    private:
        float m_MaxLifeTime;
        float m_timeAlive = 0.0f;

};