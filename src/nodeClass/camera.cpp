#include "camera.h"
#include <glm/gtx/string_cast.hpp>

Camera::Camera(wolf::App* pApp): m_pApp(pApp), m_bPauseFrustum(false){}
Camera::~Camera()
{
	printf("Destroying Camera\n");
}
void Camera::init(){
    CreateFrustumFromCamera();
    m_pApp->hideCursor(); // Added into wolf
    printf("Camera init\n");
}
void Camera::update(float dt) {
    //Create a new frustum based on new location
    
    if(!m_bPauseFrustum){
        CreateFrustumFromCamera();
        //std::cout << "creating frustum" << std::endl;
    }



    glm::vec2 screenCenter(m_pApp->getScreenSize().x / 2.0f, m_pApp->getScreenSize().y / 2.0f);
    glm::vec2 mousePos = m_pApp->getMousePos();  // Get mouse position (x, y) in window space

    float offsetX = (mousePos.x - screenCenter.x);
    float offsetY = (screenCenter.y - mousePos.y);  // Reverse Y axis

    if (m_inverted) {
        offsetY = -offsetY;  // Invert up/down
    }

    // Sensitivity settings for camera movement
    const float sensitivity = 0.05f;
    offsetX *= sensitivity; 
    offsetY *= sensitivity;

    // Update yaw and pitch based on mouse movement
    m_yaw += offsetX;
    m_pitch += offsetY;

    //Limits from player looking to high
    m_pitch = glm::clamp(m_pitch, -89.0f, 89.0f);

    // Update the front vector based on yaw and pitch
    updateCameraVectors();

    m_pApp->setMousePos(screenCenter.x, screenCenter.y); //Added in wolf

    glm::vec3 forwardXZ = glm::normalize(glm::vec3(m_front.x, 0.0f, m_front.z));
    glm::vec3 rightXZ = glm::normalize(glm::cross(forwardXZ, glm::vec3(0.0f, 1.0f, 0.0f)));

    float cameraSpeed = 2.0f * dt; // Adjust speed based on frame time (dt)
    DetectInput(cameraSpeed, forwardXZ, rightXZ);
}

void Camera::DetectInput(float &cameraSpeed, glm::vec3 &forwardXZ, glm::vec3 &rightXZ)
{
    if (m_pApp->isKeyDown(GLFW_KEY_LEFT_SHIFT))
    {
        cameraSpeed *= 4;
    }
    if (m_pApp->isKeyDown(GLFW_KEY_W))
    {
        m_position += forwardXZ * cameraSpeed;
    }
    if (m_pApp->isKeyDown(GLFW_KEY_S))
    {
        m_position -= forwardXZ * cameraSpeed;
    }
    if (m_pApp->isKeyDown(GLFW_KEY_A))
    {
        m_position -= rightXZ * cameraSpeed;
    }
    if (m_pApp->isKeyDown(GLFW_KEY_D))
    {
        m_position += rightXZ * cameraSpeed;
    }
    if (m_pApp->isKeyDown(GLFW_KEY_Q))
    {
        m_position.y -= cameraSpeed;
    }
    if (m_pApp->isKeyDown(GLFW_KEY_E))
    {
        m_position.y += cameraSpeed;
    }
    if (m_pApp->isKeyDown(GLFW_KEY_I))
    { // Press 'I' to toggle inversion
        this->invertCamera();
    }
}
void Camera::updateCameraVectors() {
    // Calculate the new front vector based on yaw and pitch
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);

    // Also re-calculate the right and up vectors
    m_right = glm::normalize(glm::cross(m_front, glm::vec3(0.0f, 1.0f, 0.0f)));  // Cross product with up vector
    m_up = glm::normalize(glm::cross(m_right, m_front));  // Up vector
}
glm::mat4 Camera::getViewMatrix(){      
    return glm::lookAt(m_position, m_position + m_front, m_up);
}
glm::mat4 Camera::getProjMatrix() {
    return glm::perspective(m_fov, (float)m_screenWidth / (float)m_screenHeight, m_near, m_far);
}
glm::vec3 Camera::getViewPosition(){
    return m_position;
}
void Camera::invertCamera(){
    m_inverted = !m_inverted;
}
void Camera::setScreenSize(glm::vec2 screenSize) {
    m_screenWidth = screenSize.x;
    m_screenHeight = screenSize.y;
}

void Camera::CreateFrustumFromCamera()
{
    float aspect = m_pApp->getScreenSize().x / m_pApp->getScreenSize().y;

    const float halfVSide = m_far * tanf(m_fov * 0.5f);
    const float halfHSide = halfVSide * aspect;
    const glm::vec3 frontMultFar = glm::vec3(m_far * m_front);

    m_frustum.nearFace = { getViewPosition() + m_near * m_front, m_front };
	m_frustum.farFace = { getViewPosition() + frontMultFar, -m_front };
	m_frustum.rightFace = { getViewPosition(), glm::cross(frontMultFar - m_right * halfHSide, m_up) };
	m_frustum.leftFace = { getViewPosition(), glm::cross(m_up, frontMultFar + m_right * halfHSide) };
	m_frustum.topFace = { getViewPosition(), glm::cross(m_right, frontMultFar - m_up * halfVSide) };
	m_frustum.bottomFace = {getViewPosition(), glm::cross(frontMultFar + m_up * halfVSide, m_right)};

}
void Camera::PauseFrustum(){
    m_bPauseFrustum = !m_bPauseFrustum;
}
