#pragma once
#include "light.h"

class DirectionalLight : public Light {
    public:
        DirectionalLight(const glm::vec3& color,
                        float intensity,
                        int priority,
                        const glm::vec3& direction)
            : Light(color, priority, intensity),
            m_direction(glm::normalize(direction))
        {}

        void update() override {
            // Update directional light properties
        }

        void render() override {
            // Render the directional light.
        }

        // Getter and setter for direction.
        glm::vec3 getDirection() const { return m_direction; }
        void setDirection(const glm::vec3& direction) { m_direction = glm::normalize(direction); }

    private:
        glm::vec3 m_direction;
};
