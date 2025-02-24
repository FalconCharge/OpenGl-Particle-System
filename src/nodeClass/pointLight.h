#pragma once
#include "light.h"

class PointLight : public Light {
public:
    PointLight(const glm::vec3& color,
               float intensity,
               int priority,
               float range,
               const glm::vec3& lightAttenuation)
        : Light(color, priority, intensity),
            m_vLightAttenuation(lightAttenuation),
            m_fRange(range)
    {}

    // Override the update and render methods.
    void update() override {
        // Update point light properties
    }

    void render() override {
        // Render the point light.
        // This might involve setting uniforms in a shader
    }

    // Getter functions for attenuation and range.
    glm::vec3 getAttenuation() const { return m_vLightAttenuation; }
    float getRange() const { return m_fRange; }

    glm::vec3 GetPosition() {return GetWorldPosition();}

    // Setter functions for attenuation and range.
    void setAttenuation(const glm::vec3& attenuation) { m_vLightAttenuation = attenuation; }
    void setRange(float range) { m_fRange = range; }

private:
    glm::vec3 m_vLightAttenuation;
    float m_fRange;
};
