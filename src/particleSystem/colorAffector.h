#pragma once
#include "affector.h"

class ColorAffector : public Affector {
    public:
        ColorAffector(const glm::vec4& startColor, const glm::vec4& endColor, std::string mode)
            : m_StartColor(startColor), m_EndColor(endColor), m_Mode(mode)  {}
    
        void Apply(Particle* particle, float deltaTime) override {
            PointBB* point = static_cast<PointBB*>(particle);
            if(m_Mode == "OverLife"){
                float t = point->GetTimeAlive() / point->GetMaxTimeAlive();
                point->SetColor(glm::mix(m_StartColor, m_EndColor, t));

            }
            else if (m_Mode == "Instant") {
                point->SetColor(m_EndColor);
            }else{
                // White color output for default
                point->SetColor(glm::vec4(1));
            }
        }
    
    private:
        glm::vec4 m_StartColor;
        glm::vec4 m_EndColor;
        std::string m_Mode;
    };
    