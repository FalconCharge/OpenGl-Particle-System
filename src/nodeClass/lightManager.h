#pragma once
#include "light.h"
#include "directionalLight.h"
#include "pointLight.h"

class LightManager {
public:
    LightManager() {}


    void AddDirectionalLight(const glm::vec3& direction, const glm::vec3& color, float ambientStrength) {
        m_directionalLight = new DirectionalLight(color, ambientStrength, 100, direction);
    }

    void AddPointLight(const glm::vec3& color, float intensity, float weight, float range, const glm::vec3& lightAttenuation){
        m_pointLight = new PointLight(color, intensity, weight, range, lightAttenuation);
    }

    void UpdateLights() {
        
    }

    // Get the directional light
    const DirectionalLight& GetDirectionalLight() const {
        return *m_directionalLight;
    }

    // Get the point light
    PointLight& GetPointLight() {
        return * m_pointLight;
    }

private:
    DirectionalLight* m_directionalLight;    // Usually only have 1 of these
    PointLight* m_pointLight;
};
