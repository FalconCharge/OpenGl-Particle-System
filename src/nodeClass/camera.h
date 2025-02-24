#pragma once
#include "../../wolf/wolf.h"
#include "glm/glm.hpp"
#include <iostream>
#include <glm/gtx/string_cast.hpp>  // for prinitng


    struct Plane{
        glm::vec3 normal = {0.0f, 1.0f, 0.0f};

        float distance = 0.0f;

        Plane() = default;

        Plane(const glm::vec3& p1, const glm::vec3& norm)
            : normal(glm::normalize(norm)), distance(glm::dot(normal, p1)){}

        float GetSignedDistanceToPlane(const glm::vec3& point)
        {
            return glm::dot(normal, point) - distance;
        }
    };

    struct Frustum
    {
        Plane topFace;
        Plane bottomFace;

        Plane rightFace;
        Plane leftFace;

        Plane farFace;
        Plane nearFace;
    };

class Camera
{
public:


    Camera(wolf::App* pApp);
    virtual ~Camera();

    void update(float dt);
    void DetectInput(float &cameraSpeed, glm::vec3 &forwardXZ, glm::vec3 &rightXZ);
    void init();
    void invertCamera();
    void setScreenSize(glm::vec2 screenSize);
    void SetFOV(float fov){this->m_fov = fov;}
    void SetNear(float nearPlane){this->m_near = nearPlane;}
    void SetFar(float farPlane){this->m_far = farPlane;}
    void SetPosition(glm::vec3 position){this->m_position = position;}

    void PauseFrustum();

    
    glm::mat4 getViewMatrix();
    glm::mat4 getProjMatrix();
    glm::vec3 getViewPosition();

    void CreateFrustumFromCamera();
    void DebugDrawFrustum();    // Be nice to have 
    Frustum& GetFrustum(){return m_frustum;}


private:

    // Private variables for camera properties
    glm::vec3 m_position = glm::vec3(0.0f, 10.0f, 0.0f);  // Camera position
    glm::vec3 m_front = glm::vec3(0.0f, 0.0f, -1.0f);     // Front direction vector
    glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);         // Up direction vector
    glm::vec3 m_right = glm::vec3(1.0f, 0.0f, 0.0f);       // Right direction vector
    
    // Camera angles
    float m_yaw = -90.0f;  // Yaw, in degrees, initially pointing to -Z
    float m_pitch = 0.0f;  // Pitch, in degrees
    float m_fov = glm::radians(45.0f);  // Field of view
    float m_near = 0.1f;   // Near clipping plane
    float m_far = 1000.0f; // Far clipping plane
    bool m_inverted = false;

    Frustum m_frustum;


    int m_screenWidth = 800;
    int m_screenHeight = 600;

    wolf::App* m_pApp = nullptr;  // Pointer to the application (wolf::App)
    
    // Helper function to update the camera's front, right, and up vectors
    void updateCameraVectors();
    
    bool m_bPauseFrustum = false;
};
