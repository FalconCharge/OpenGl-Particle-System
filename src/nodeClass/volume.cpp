#pragma once
#include "volume.h"


bool Sphere::IsOnOrForwardPlane(Plane& plane)
{
    return plane.GetSignedDistanceToPlane(m_center) > -m_radius;
}

bool Sphere::IsOnFrustum(Frustum* camFrustum, Node& transform)
{
    //compute global scale from Node
    glm::vec3 globalScale = glm::vec3(  glm::length(transform.GetWorldTransform()[0]), 
                                        glm::length(transform.GetWorldTransform()[1]),
                                        glm::length(transform.GetWorldTransform()[2]));

    //Get our global center with process it with the global model matrix of our transform
    glm::vec3 globalCenter{ transform.GetWorldTransform() * glm::vec4(m_center, 1.f) };

    //To wrap correctly our shape, we need the maximum scale scalar.
    float maxScale = std::max(std::max(globalScale.x, globalScale.y), globalScale.z);

    //Max scale is assuming for the diameter. So, we need the half to apply it to our radius
    Sphere globalSphere(globalCenter, m_radius * (maxScale * 0.5f));

    bool result = globalSphere.IsOnOrForwardPlane(camFrustum->leftFace);

    
    //Check Firstly the result that have the most chance to failure to avoid to call all functions.
    return (globalSphere.IsOnOrForwardPlane(camFrustum->leftFace) &&
        globalSphere.IsOnOrForwardPlane(camFrustum->rightFace) &&
        globalSphere.IsOnOrForwardPlane(camFrustum->farFace) &&
        globalSphere.IsOnOrForwardPlane(camFrustum->nearFace) &&
        globalSphere.IsOnOrForwardPlane(camFrustum->topFace) &&
        globalSphere.IsOnOrForwardPlane(camFrustum->bottomFace));
	
}
void Sphere::Render(glm::mat4& mView, glm::mat4& mProj, glm::mat4& world){
    // Not implemented
    return;
}

void Sphere::Debug() {
    std::cout << "Sphere Debug Info:\n";
    std::cout << "Center: " << glm::to_string(m_center) << "\n";
    std::cout << "Radius: " << m_radius << "\n";
}
/*AABB::AABB(const glm::vec3& inCenter, float inExtent) : Volume{}, m_center{inCenter}, m_extent{inExtent}{
    m_size = glm::vec3(inExtent);
}
AABB::AABB(glm::vec3& inCenter, glm::vec3& size){
    m_center = inCenter;
    m_extent = size.x; // Just pick any side for the extent
    m_size = size;
}
bool AABB::IsOnOrForwardPlane(Plane& plane){
    // Compute the projection interval radius of b onto L(t) = b.c + t * p.n
    float r = m_extent * (std::abs(plane.normal.x) + std::abs(plane.normal.y) + std::abs(plane.normal.z));
    return -r <= plane.GetSignedDistanceToPlane(m_center);
}
bool AABB::IsOnFrustum(Frustum* camFrustum, Node& transform){
    //Get global scale thanks to our transform
    glm::vec3 globalCenter{ transform.getWorldTransform() * glm::vec4(m_center, 1.0f) };

    // Scaled orientation
    glm::vec3 right = transform.getWorldTransform()[0] * m_extent;
    glm::vec3 up = transform.getWorldTransform()[1] * m_extent;
    glm::vec3 forward = -transform.getWorldTransform()[2] * m_extent;

    float newIi = std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, right)) +
        std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, up)) +
        std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, forward));

    float newIj = std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, right)) +
        std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, up)) +
        std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, forward));

    float newIk = std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, right)) +
        std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, up)) +
        std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, forward));

    AABB globalAABB(globalCenter, std::max(std::max(newIi, newIj), newIk));

    return (globalAABB.IsOnOrForwardPlane(camFrustum->leftFace) &&
            globalAABB.IsOnOrForwardPlane(camFrustum->rightFace) &&
            globalAABB.IsOnOrForwardPlane(camFrustum->topFace) &&
            globalAABB.IsOnOrForwardPlane(camFrustum->bottomFace) &&
            globalAABB.IsOnOrForwardPlane(camFrustum->nearFace) &&
            globalAABB.IsOnOrForwardPlane(camFrustum->farFace));
};

void AABB::Debug() {
    std::cout << "Sphere Debug Info:\n";
    std::cout << "Center: " << glm::to_string(m_center) << "\n";
    std::cout << "Extent: " << m_extent << "\n";
}
void AABB::Render(glm::mat4& mView, glm::mat4& mProj, glm::mat4& mWorld){

    m_pProgram->Bind();
    
    glm::mat4 MVP = mProj * mView * mWorld;

    m_pProgram->SetUniform("MVP", MVP);

    m_pProgram->SetUniform("color", glm::vec4(1, 0, 0, 1));

    m_pDecl->Bind();

    // Draw!
    glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_SHORT, nullptr);

}
void AABB::SetupRendering(glm::mat4& worldTransform){
    //m_worldTransform = worldTransform;

    //setting up corners
    glm::vec3 minPoint = m_center - m_extent;
    glm::vec3 maxPoint = m_extent + m_center;

    normalVertices = {
        {minPoint.x, minPoint.y, minPoint.z}, // 0
        {maxPoint.x, minPoint.y, minPoint.z}, // 1
        {minPoint.x, maxPoint.y, minPoint.z}, // 2
        {maxPoint.x, maxPoint.y, minPoint.z}, // 3
        {minPoint.x, minPoint.y, maxPoint.z}, // 4
        {maxPoint.x, minPoint.y, maxPoint.z}, // 5
        {minPoint.x, maxPoint.y, maxPoint.z}, // 6
        {maxPoint.x, maxPoint.y, maxPoint.z}  // 7
    };

    // setting up connctions
    indices = {
        0, 1,  // bottom face
        1, 3,
        3, 2,
        2, 0,

        4, 5,  // top face
        5, 7,
        7, 6,
        6, 4,

        0, 4,  // vertical edges
        1, 5,
        2, 6,
        3, 7
    };

    // Shader creation
    
    m_pProgram = wolf::ProgramManager::CreateProgram("Data/Shader/grid.vsh", "Data/Shader/grid.fsh");
    if(m_pProgram == nullptr){
        std::cout << "[ERROR] Shader did not load properly" << std::endl;
    }

    // Create VB and Decl
    m_pVB = wolf::BufferManager::CreateVertexBuffer(normalVertices.data(), normalVertices.size() * sizeof(AABBVertex));
    m_pIB = wolf::BufferManager::CreateIndexBuffer(indices.data(), indices.size());

    m_pDecl = new wolf::VertexDeclaration();

    m_pDecl->Begin();
    m_pDecl->AppendAttribute(wolf::AT_Position, 3, wolf::CT_Float);     // Pos attr
    m_pDecl->SetVertexBuffer(m_pVB);
    m_pDecl->SetIndexBuffer(m_pIB);
    m_pDecl->End();

}

*/