#pragma once
#include "affector.h"

#include <string>


class ScaleAffector : public Affector{
    public:
        ScaleAffector(float startScale, float endScale, std::string mode) : m_StartScale(startScale), m_EndScale(endScale), m_Mode(mode) {}

        ~ScaleAffector() {}

        void Apply(Particle* particle, float p_fDeltaTime){
            PointBB* point = static_cast<PointBB*>(particle);
            if(m_Mode == "OverLife"){
                float t = point->GetTimeAlive() / point->GetMaxTimeAlive();
                point->SetSize(glm::mix(m_StartScale, m_EndScale, t));
            }
            else if (m_Mode == "Instant") {
                point->SetSize(m_EndScale);
            }else if (m_Mode == "Breath") {
                // Breathing effect using a sine wave
                float t = point->GetTimeAlive() / point->GetMaxTimeAlive();
                float oscillation = std::sin(t * 2 * PI);
                float scale = glm::mix(m_StartScale, m_EndScale, (oscillation + 1.0f) / 2.0f); // Map sine wave to [start, end]
                point->SetSize(scale);
            }
        }

    private:
        float m_StartScale;
        float m_EndScale;
        std::string m_Mode;

};