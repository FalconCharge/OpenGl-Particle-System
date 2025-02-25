#include "CircleShapeParticle.h"

#include <cmath>

CircleShapeParticle::CircleShapeParticle(){

}
CircleShapeParticle::CircleShapeParticle(glm::vec3 position){
    SetLocalPosition(position);
}
CircleShapeParticle::~CircleShapeParticle(){
    
}
void CircleShapeParticle::Init(){

    //Init the particles with there vertex points
    pPBB1 = new PointBB(glm::vec3(0, 0, 0));
    pPBB1->SetVertices(GenerateCirclePoints(400000));
    pPBB1->SetName("Most Dense");
    pPBB1->SetColor(glm::vec4(0, 0, 0, 1)); // Black


    pPBB2 = new PointBB(glm::vec3(0, 0, 0));
    pPBB2->SetVertices(GenerateCirclePoints(2000));
    pPBB2->SetName("Mid Dense");
    pPBB2->SetColor(glm::vec4(0, 0, 0, 1)); // Black


    pPBB3 = new PointBB(glm::vec3(0, 0, 0));
    pPBB3->SetVertices(GenerateCirclePoints(1000));
    pPBB3->SetName("Least Dense");
    pPBB3->SetColor(glm::vec4(0, 0, 0, 1)); // Black



    pPBB1->SetTexture("Data/textures/particles/smoke_01.png");
    pPBB1->SetShader("Data/Shader/glPoint.vsh", "Data/Shader/glPoint.fsh");
    pPBB2->SetTexture("Data/textures/particles/smoke_01.png");
    pPBB2->SetShader("Data/Shader/glPoint.vsh", "Data/Shader/glPoint.fsh");
    pPBB3->SetTexture("Data/textures/particles/smoke_01.png");
    pPBB3->SetShader("Data/Shader/glPoint.vsh", "Data/Shader/glPoint.fsh");

    this->AddChild(pPBB1);
    this->AddChild(pPBB2);
    this->AddChild(pPBB3);

    pPBB1->Init();
    pPBB2->Init();
    pPBB3->Init();

    SetVolume(CalculateVolume());


}
void CircleShapeParticle::Update(float p_fDelta){
    Node::Update(p_fDelta);   // Not sure if this should be here or not

}
void CircleShapeParticle::Render(glm::mat4 proj, glm::mat4 view){
    Node::Render(proj, view);
}

AABB& CircleShapeParticle::CalculateVolume() {
    // Comments are all stolen from debug cube
    // For now made is the BillBoard is Huge so we don't need to calculate and It will always be in the frustum
    // Hopefully


    /* ERROR IN GetWorldTransform somewhere but I gotta finish this assignment
    // So I'm just going to work around it 
    // Typically we would pass in the transform but instead we are going to make a new transform
    // That has the correct values
    */
    //glm::mat4 transform = glm::mat4(1);
    //transform = glm::translate(transform, this->GetWorldPosition()) * glm::scale(transform, this->GetWorldScale());
    // Will transform to the center value so we want to set our trasnfor to that
    //m_bounds = AABB(transform);
    m_bounds = AABB(glm::vec3(0, 0, 0), 1000);
    return m_bounds;
}

// Pretty complex
std::vector<PointBBVertex> CircleShapeParticle::GenerateCirclePoints(int p_iNumOfParticles){

    //Generate a random X, y, z position within the circle
    std::vector<PointBBVertex> newPoints;
    newPoints.reserve(p_iNumOfParticles);

    for (int i = 0; i < p_iNumOfParticles; i++) {
        // Random spherical coordinates
        float theta = static_cast<float>(rand()) / RAND_MAX * 2.0f * PI; // Random azimuthal angle (0 to 2π)
        float phi = static_cast<float>(rand()) / RAND_MAX * PI;          // Random polar angle (0 to π)
        float radius = 5.0f;

        // Convert spherical to Cartesian coordinates
        float x = radius * std::sin(phi) * std::cos(theta);
        float y = radius * std::sin(phi) * std::sin(theta);
        float z = radius * std::cos(phi);

        newPoints.push_back({x, y, z});
    }



    //Turn them values in to a 
    return newPoints;
}

