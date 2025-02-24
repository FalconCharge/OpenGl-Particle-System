#pragma once
#include <glm/glm.hpp>
#include "node.h"

class Light : public Node{
    public:
        Light(const glm::vec3& color, int priority, float intensity)
        : m_vColor(color), m_iPriority(priority), m_iIntensity(intensity) {}
    
        virtual ~Light() = default;

        virtual void update() = 0;
        virtual void render() = 0;
        virtual AABB& CalculateVolume(){return m_bounds;}

        // Getters
        const glm::vec3& GetColor() const {return m_vColor;}
        float GetIntensity() const {return m_iIntensity;}
        int GetPriority() const {return m_iPriority;}

        // Setters
        void SetColor(glm::vec3 color){m_vColor = color;}
        void SetIntensity(float intensity){m_iIntensity = intensity;}
        void SetPriority(int priority){m_iPriority = priority;}

    protected:
        glm::vec3 m_vColor;
        int m_iPriority;    
        float m_iIntensity;
        AABB m_bounds;
};