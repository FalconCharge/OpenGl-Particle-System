#pragma once
#include "scene.h"
#include "node.h"
#include <glm/gtx/string_cast.hpp>


class Node; // Forward decl

/*struct AABBVertex{
    GLfloat x, y, z;
};*/

struct Volume{
    virtual bool IsOnFrustum(Frustum* camFrustum, Node& node) = 0;

	virtual bool IsOnOrForwardPlane(Plane& plane) = 0;
    virtual void Render(glm::mat4& mView, glm::mat4& mProj, glm::mat4& mWorld) = 0;

};

struct Sphere : public Volume
{
    glm::vec3 m_center{ 0.0f, 0.0f, 0.0f };
    float m_radius{ 0.0f };

    Sphere(const glm::vec3& inCenter, float inRadius)
		: Volume{}, m_center{ inCenter }, m_radius{ inRadius }
	{}

    virtual bool IsOnOrForwardPlane(Plane& plane) override;
    virtual bool IsOnFrustum(Frustum* camFrustum, Node& node) override;
    virtual void Render(glm::mat4& mView, glm::mat4& mProj, glm::mat4& world) override;

    void Debug();
};
/*
struct AABB : public Volume
{
    glm::vec3 m_center{0.0f, 0.0f, 0.0f}; 
    float m_extent{0.0f};
    glm::vec3 m_size = glm::vec3(0);

    // Commenting this out to break it
    //AABB(const glm::vec3& inCenter, float inExtent);
    //AABB(glm::vec3& inCenter, glm::vec3& inSize);

    glm::vec3 GetSize(){return m_size;}

    virtual bool IsOnOrForwardPlane(Plane& plane) override;
    virtual bool IsOnFrustum(Frustum* camFrustum, Node& node) override;

    void Debug();
    virtual void Render(glm::mat4& mView, glm::mat4& mProj, glm::mat4& world) override;
    void SetupRendering(glm::mat4& worldPos);

    // Render needs
    wolf::Program* m_pProgram = nullptr;
    wolf::VertexBuffer* m_pVB = nullptr;
    wolf::IndexBuffer* m_pIB = nullptr;
    wolf::VertexDeclaration* m_pDecl = nullptr;

    std::vector<AABBVertex> normalVertices;
    std::vector<unsigned short> indices;
};
*/
