#pragma once

#include "../../wolf/wolf.h"
#include "../nodeClass/scene.h"
#include "emitter.h"
#include "particle.h"

struct SpawnProperties{
    glm::vec4 color;
    glm::vec4 velocity;

};
class PointBB : public Particle{

    public:
        // Constructor
        PointBB();

        // Deconstructor
        ~PointBB();

        // Initlizes the values for the BB
        void Init();

        void Update(float p_fDelta);

        // Setters
        void SetPosition(glm::vec3 position){m_position = position;}
        void SetRotation(float rotation){m_rotation = rotation;}
        void SetScale(float scale){m_scale = scale;}
        void SetSize(float size){m_size = size;}
        void SetColor(glm::vec4 color){m_color = color;}
        void SetVelocity(glm::vec3 velocity){m_velocity = velocity;}
        void SetIsExpired(bool expired){m_isExpired = expired;}
        void ResetPoint();
        void SetSpawnProperties();
        void SetMaxTimeAlive(float maxtimeAlive){m_maxTimeAlive = maxtimeAlive;}
        void SetCameraDistance(float distance){cameraDistance = distance;}

        

        // Getters
        glm::vec3 GetPosition(){return m_position;}
        float GetScale(){return m_scale;}
        float GetRotation(){return m_rotation;}
        glm::vec3 GetVelocity(){return m_velocity;}
        float GetSize(){return m_size;}
        bool IsExpired(){return m_isExpired;}
        glm::vec4 GetColor(){return m_color;}
        float GetTimeAlive(){return m_timeAlive;}
        float GetMaxTimeAlive(){return m_maxTimeAlive;}
        float GetCameraDistance(){return cameraDistance;}

            // Comparison operator for sorting
        bool operator<(Particle* that) {
            // Sort in reverse order: far particles drawn first
            return this->cameraDistance > static_cast<PointBB*>(that)->cameraDistance;
        }

    private:
        glm::vec3 m_position = glm::vec3(0);
        float m_scale = 1000.0f;
        float m_rotation = 0;
        glm::vec3 m_velocity = glm::vec3(0);
        glm::vec4 m_color = glm::vec4(1);

        float m_size = 20;

        float m_timeAlive = 0.0f;
        float m_maxTimeAlive = 30.0f;

        bool m_isExpired = false;

        float cameraDistance;


};