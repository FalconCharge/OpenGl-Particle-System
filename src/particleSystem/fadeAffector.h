#pragma once
#include "affector.h"

#include <string>


class FadeAffector : public Affector{
    public:
        FadeAffector(float startAlpha, float midAlpha, float endAlpha, std::string mode) : m_StartAlpha(startAlpha), m_EndAlpha(endAlpha),
                     m_MidAlpha(midAlpha), m_Mode(mode) {}

        ~FadeAffector() {}

        void Apply(Particle* particle, float p_fDeltaTime){
            PointBB* point = static_cast<PointBB*>(particle);
            if(m_Mode == "OverLife"){
                float t = point->GetTimeAlive() / point->GetMaxTimeAlive();

                float newAlpha;
                if (t < 0.5f) { 
                    // First half of life: Interpolate from start to mid
                    newAlpha = glm::mix(m_StartAlpha, m_MidAlpha, t * 2.0f);
                } else { 
                    // Second half of life: Interpolate from mid to end
                    newAlpha = glm::mix(m_MidAlpha, m_EndAlpha, (t - 0.5f) * 2.0f);
                }
                point->SetFade(newAlpha);

            }
            else if (m_Mode == "Instant") {
                point->SetFade(m_EndAlpha);
            }
        }

    private:
        float m_StartAlpha;
        float m_EndAlpha;
        float m_MidAlpha;
        std::string m_Mode;

};