#pragma once

#include "../../wolf/wolf.h"
#include "../nodeClass/scene.h"
#include "emitter.h"
#include "particle.h"


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
        void SetRotation(glm::vec3 rotation){m_rotation = rotation;}
        void SetScale(glm::vec3 scale){m_scale = scale;}
        void SetSize(float size){m_size = size;}
        void SetVelocity(glm::vec3 velocity){m_velocity = velocity;}
        void SetIsExpired(bool expired){m_isExpired = expired;}
        void ResetPoint();

        

        // Getters
        glm::vec3 GetPosition(){return m_position;}
        glm::vec3 GetScale(){return m_scale;}
        glm::vec3 GetRotation(){return m_rotation;}
        glm::vec3 GetVelocity(){return m_velocity;}
        float GetSize(){return m_size;}
        bool IsExpired(){return m_isExpired;}



    private:
        glm::vec3 m_position;
        glm::vec3 m_scale;
        glm::vec3 m_rotation;
        glm::vec3 m_velocity;

        float m_size = 20;

        float m_timeAlive = 0.0f;
        float m_maxTimeAlive = 3.0f;

        bool m_isExpired = false;


};