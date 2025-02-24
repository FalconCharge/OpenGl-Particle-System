#pragma once

#include "scene.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>  // For glm::to_string
#include "node.h"
/*
// I'm remaking the AABB class the old one is too complex
*/
struct AABBVertex{
    GLfloat x, y, z;
};
class Node;


class AABB{
    public:
        // Default constructor
        AABB(){};
        // Constructor Not used in Project
        AABB(const glm::vec3& inCenter, glm::vec3& inSize);
        // Constructor Not used in Project
        AABB(glm::vec3& inCenter, float inExtent);
        // Main constructor used
        AABB(glm::mat4& inTransform);
        // Deconstructor
        ~AABB();

        // Expand the AABB to include the point; Not tested
        void Encapsulate(glm::vec3 point);
        // Gets the closest point on the AABB to inPoint
        glm::vec3 ClosestPoint(glm::vec3 point);
        // Returns if the AABB contains inPoint
        bool Contains(glm::vec3 point) const;
        // Returns true if any corner is contained in the AABB
        bool Contains(const glm::vec3& min, const glm::vec3& max) const;
        // Returns true if the AABB fully contains inAABB
        bool FullyContains(const AABB& other) const;
        // Expands the AABB by a certain amount
        void Expand(glm::vec3 amount);
        // Returns true if inAABB intersects with this.AABB
        bool Intersects(AABB bounds);
        // Sets the Min and Max values;
        void SetMinMax(glm::vec3 min, glm::vec3 max);
        // A Debug For AABB
        void Debug() const;

        // Renders the AABB with gl Lines
        void Render(glm::mat4& mView, glm::mat4& mProj) const;

        // A method found online from openGL Learn section frustum culling
        bool IsOnOrForwardPlane(Plane& plane);
        // A Method found online from openGL Learn section frustum culling
        bool IsOnFrustum(Frustum& camFrustum, Node& node);
        // Retuns true if the current AABB is in the inFrustum
        bool IsOnFrustum(Frustum& camFrustum);
        // Sets the colors of the rendering lines
        void SetColor(glm::vec3 color){m_vColor = color;};

        //Getters
        glm::vec3 GetCenter() const {return m_center;}
        glm::vec3 GetSize() const {return m_size;}
        glm::vec3 GetMax() const {return m_max;}
        glm::vec3 GetMin()const {return m_min;}
        float GetExtent()const{return m_extent;}
        glm::vec3 GetColor(){return m_vColor;};



    private:
        glm::vec3 m_center; // The center
        float m_extent;    // Half of the size
        glm::vec3 m_size;     // Size of the AABB
        //Corners
        glm::vec3 m_max;
        glm::vec3 m_min;

        glm::vec3 m_vColor;

        glm::mat4 m_transform;


        // Rendering needs
        void SetupRendering();

        // Render needs
        wolf::Program* m_pProgram = nullptr;
        wolf::VertexBuffer* m_pVB = nullptr;
        wolf::IndexBuffer* m_pIB = nullptr;
        wolf::VertexDeclaration* m_pDecl = nullptr;

        std::vector<AABBVertex> normalVertices;
        std::vector<unsigned short> indices;
};