#pragma once
#include "affector.h"

// Moves particles to the left (negative x direction)
class MoveLeftAffector : public Affector {
public:
    MoveLeftAffector(float speed) : m_speed(speed) {}
    ~MoveLeftAffector() override = default;
    
    void Apply(Particle* particle, float deltaTime) override {
        PointBB* point = static_cast<PointBB*>(particle);
        glm::vec3 pos = point->GetPosition();
        pos.x -= m_speed * deltaTime;
        point->SetPosition(pos);
    }
    
private:
    float m_speed;
};

// Moves particles to the right (positive x direction)
class MoveRightAffector : public Affector {
public:
    MoveRightAffector(float speed) : m_speed(speed) {}
    ~MoveRightAffector() override {};
    
    void Apply(Particle* particle, float deltaTime) override {
        PointBB* point = static_cast<PointBB*>(particle);
        glm::vec3 pos = point->GetPosition();
        pos.x += m_speed * deltaTime;
        point->SetPosition(pos);
    }
    
private:
    float m_speed;
};

// Moves particles upward (positive y direction)
class MoveUpAffector : public Affector {
public:
    MoveUpAffector(float speed) : m_speed(speed) {}
    ~MoveUpAffector() override = default;
    
    void Apply(Particle* particle, float deltaTime) override {
        PointBB* point = static_cast<PointBB*>(particle);
        glm::vec3 pos = point->GetPosition();
        pos.y += m_speed * deltaTime;
        point->SetPosition(pos);
    }
    
private:
    float m_speed;
};

// Moves particles downward (negative y direction)
class MoveDownAffector : public Affector {
public:
    MoveDownAffector(float speed) : m_speed(speed) {}
    ~MoveDownAffector() override = default;
    
    void Apply(Particle* particle, float deltaTime) override {
        PointBB* point = static_cast<PointBB*>(particle);
        glm::vec3 pos = point->GetPosition();
        pos.y -= m_speed * deltaTime;
        point->SetPosition(pos);
    }
    
private:
    float m_speed;
};

// Moves particles forward (assuming forward is negative z)
class MoveForwardAffector : public Affector {
public:
    MoveForwardAffector(float speed) : m_speed(speed) {}
    ~MoveForwardAffector() override = default;
    
    void Apply(Particle* particle, float deltaTime) override {
        PointBB* point = static_cast<PointBB*>(particle);
        glm::vec3 pos = point->GetPosition();
        pos.z -= m_speed * deltaTime;
        point->SetPosition(pos);
    }
    
private:
    float m_speed;
};

// Moves particles backward (assuming backward is positive z)
class MoveBackwardAffector : public Affector {
public:
    MoveBackwardAffector(float speed) : m_speed(speed) {}
    ~MoveBackwardAffector() override = default;
    
    void Apply(Particle* particle, float deltaTime) override {
        PointBB* point = static_cast<PointBB*>(particle);
        glm::vec3 pos = point->GetPosition();
        pos.z += m_speed * deltaTime;
        point->SetPosition(pos);
    }
    
private:
    float m_speed;
};
