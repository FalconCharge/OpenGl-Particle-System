#include "aabb.h"

#pragma region Rendering
void AABB::Render(glm::mat4& mView, glm::mat4& mProj) const{

    m_pProgram->Bind();

    glm::mat4 MVP = mProj * mView;

    m_pProgram->SetUniform("MVP", MVP);

    m_pProgram->SetUniform("color", glm::vec4(m_vColor, 1));

    //m_pDecl->Bind();

    // Draw!    [Error] not sure what's wrong here
    //glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_SHORT, nullptr);

}

void AABB::SetupRendering(){
    glm::vec3 minPoint(-0.5f, -0.5f, -0.5f);
    glm::vec3 maxPoint(0.5f, 0.5f, 0.5f);

    std::vector<glm::vec3> localVertices = {
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

    // Transform each vertex to world space
    normalVertices.clear();
    for (const auto& v : localVertices) {
        glm::vec4 worldV = m_transform * glm::vec4(v, 1.0f); 
        AABBVertex transformedVertex;
        transformedVertex.x = worldV.x;
        transformedVertex.y = worldV.y;
        transformedVertex.z = worldV.z;

        normalVertices.push_back(transformedVertex);
    }

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
#pragma endregion

// Constructor
AABB::AABB(const glm::vec3& inCenter, glm::vec3& inSize): m_center(inCenter), m_size(inSize)
{
    m_extent = inSize.x * 0.5f; 
    // Compute min and max corners
    m_min = m_center - (m_size * 0.5f);
    m_max = m_center + (m_size * 0.5f);

    SetupRendering();
}
AABB::AABB(glm::vec3& inCenter, float inExtents) : m_extent(inExtents), m_center(inCenter)
{
    m_size = glm::vec3(inExtents * 2);

    // Compute min and max corners
    m_min = m_center - (m_size * 0.5f);
    m_max = m_center + (m_size * 0.5f);


    SetupRendering();
}
AABB::AABB(glm::mat4& inTransform) {

    // Extract the world position (translation) from the transform matrix
    m_center = glm::vec3(inTransform[3]); // Column 3 holds the translation


    glm::vec3 scaleVec(glm::length(glm::vec3(inTransform[0])),
                       glm::length(glm::vec3(inTransform[1])),
                       glm::length(glm::vec3(inTransform[2])));
    float scale = glm::max(scaleVec.x, glm::max(scaleVec.y, scaleVec.z)); // Use max scale


    // Compute extents and size
    m_extent = scale * 0.5f;
    m_size = glm::vec3(m_extent * 2.0f);

    // Compute min and max corners
    m_min = m_center - glm::vec3(m_extent);
    m_max = m_center + glm::vec3(m_extent);

    m_transform = inTransform;

    SetupRendering();
}
AABB::~AABB(){
    //delete m_pDecl;
    //wolf::BufferManager::DestroyBuffer(m_pIB);
    //wolf::BufferManager::DestroyBuffer(m_pVB);
    //wolf::ProgramManager::DestroyProgram(m_pProgram);
}



void AABB::Debug() const
{
    std::cout << "--------------------------" << std::endl;
    std::cout << "AABB Debug Information:" << std::endl;
    std::cout << "Center: " << glm::to_string(m_center) << std::endl;
    std::cout << "Size:   " << glm::to_string(m_size) << std::endl;
    std::cout << "Extent: " << m_extent << std::endl;
    std::cout << "Min:    " << glm::to_string(m_min) << std::endl;
    std::cout << "Max:    " << glm::to_string(m_max) << std::endl;
    std::cout << "--------------------------" << std::endl;
}

bool AABB::IsOnOrForwardPlane(Plane& plane){
    
    // Compute the projection interval radius of b onto L(t) = b.c + t * p.n
    float r = m_extent * (std::abs(plane.normal.x) + std::abs(plane.normal.y) + std::abs(plane.normal.z));
    return -r <= plane.GetSignedDistanceToPlane(m_center);
    
}
bool AABB::IsOnFrustum(Frustum& camFrustum, Node& transform){
    //Get global scale thanks to our transform
    glm::vec3 globalCenter{ transform.GetWorldTransform() * glm::vec4(m_center, 1.0f) };

    // Scaled orientation
    glm::vec3 right = transform.GetWorldTransform()[0] * m_extent;
    glm::vec3 up = transform.GetWorldTransform()[1] * m_extent;
    glm::vec3 forward = -transform.GetWorldTransform()[2] * m_extent;

    float newIi = std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, right)) +
        std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, up)) +
        std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, forward));

    float newIj = std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, right)) +
        std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, up)) +
        std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, forward));

    float newIk = std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, right)) +
        std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, up)) +
        std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, forward));

    AABB globalAABB(globalCenter, std::max({newIi, newIj, newIk}));

    return (globalAABB.IsOnOrForwardPlane(camFrustum.leftFace) &&
            globalAABB.IsOnOrForwardPlane(camFrustum.rightFace) &&
            globalAABB.IsOnOrForwardPlane(camFrustum.topFace) &&
            globalAABB.IsOnOrForwardPlane(camFrustum.bottomFace) &&
            globalAABB.IsOnOrForwardPlane(camFrustum.nearFace) &&
            globalAABB.IsOnOrForwardPlane(camFrustum.farFace));
};
bool AABB::IsOnFrustum(Frustum& camFrustum){
    return (IsOnOrForwardPlane(camFrustum.leftFace) &&
            IsOnOrForwardPlane(camFrustum.rightFace) &&
            IsOnOrForwardPlane(camFrustum.topFace) &&
            IsOnOrForwardPlane(camFrustum.bottomFace) &&
            IsOnOrForwardPlane(camFrustum.nearFace) &&
            IsOnOrForwardPlane(camFrustum.farFace));
}

// Expand the AABB to include the given point
void AABB::Encapsulate(glm::vec3 point)
{
    // Update min and max corners
    m_min = glm::min(m_min, point);
    m_max = glm::max(m_max, point);

    // Recalculate center and size from the updated bounds
    m_center = (m_min + m_max) * 0.5f;
    m_size = m_max - m_min;

    // Update extent (here still assuming a cube in x direction only)
    m_extent = m_size.x * 0.5f;
}
// Determines if the given point is contained in the AABB
bool AABB::Contains(glm::vec3 point) const
{
    return (point.x >= m_min.x && point.x <= m_max.x &&
            point.y >= m_min.y && point.y <= m_max.y &&
            point.z >= m_min.z && point.z <= m_max.z);
}
bool AABB::Contains(const glm::vec3& min, const glm::vec3& max) const{
    glm::vec3 corners[8] = {
        glm::vec3(min.x, min.y, min.z), // min corner
        glm::vec3(max.x, min.y, min.z), // x-max corner
        glm::vec3(min.x, max.y, min.z), // y-max corner
        glm::vec3(max.x, max.y, min.z), // xy-max corner
        glm::vec3(min.x, min.y, max.z), // z-max corner
        glm::vec3(max.x, min.y, max.z), // xz-max corner
        glm::vec3(min.x, max.y, max.z), // yz-max corner
        glm::vec3(max.x, max.y, max.z)  // xyz-max corner
    };

    // Check if any of the corners are inside the AABB
    for (const glm::vec3& corner : corners) {
        if (corner.x >= m_min.x && corner.x <= m_max.x &&
            corner.y >= m_min.y && corner.y <= m_max.y &&
            corner.z >= m_min.z && corner.z <= m_max.z) {
            return true; // Return true if any corner is inside
        }
    }

    return false; // Return false if no corner is inside
}

bool AABB::FullyContains(const AABB& other) const{
    return (other.m_min.x >= m_min.x && other.m_max.x <= m_max.x &&
            other.m_min.y >= m_min.y && other.m_max.y <= m_max.y &&
            other.m_min.z >= m_min.z && other.m_max.z <= m_max.z);
}
// Determines if this AABB intersects with another AABB
bool AABB::Intersects(AABB bounds)
{
    bool noOverlap = (m_max.x < bounds.m_min.x || m_min.x > bounds.m_max.x) ||
                     (m_max.y < bounds.m_min.y || m_min.y > bounds.m_max.y) ||
                     (m_max.z < bounds.m_min.z || m_min.z > bounds.m_max.z);
    return !noOverlap;
}
// Sets a new max and min for the corners
void AABB::SetMinMax(glm::vec3 min, glm::vec3 max)
{
    m_min = min;
    m_max = max;
    m_center = (m_min + m_max) * 0.5f;
    m_size = m_max - m_min;
    m_extent = m_size.x * 0.5f;
}