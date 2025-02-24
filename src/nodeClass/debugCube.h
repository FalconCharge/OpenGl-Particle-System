#pragma once
#include "scene.h"
#include <glm/glm.hpp>
#include "../../wolf/wolf.h"

#include <vector>


class DebugCube : public Node{

    public:
        //Constructor with default values
        DebugCube(glm::vec3 position = glm::vec3(0.0f), glm::vec3 = glm::vec3(1.0f), glm::vec3 rotation = glm::vec3(0.0f));
        ~DebugCube();

        // Initalizes the cube
        void Init();

        void Render(glm::mat4 proj, glm::mat4 view) override;
        void Update(float fDelta) override;

        void setShader(wolf::Program* shader){m_pProgram = shader;}
        
        void SetShader(const std::string& vshPath, const std::string& fshPath);
        void SetTexture(const std::string& path);

        // Turns off rendering the AABBs
        void renderBB(){m_renderBB = !m_renderBB;}

        // Calculates the Volume
        AABB& CalculateVolume() override;


    private:
        struct CubeVertex{
            GLfloat x,y,z;       // Position Cords
            GLfloat u,v;         // Texture Cords
            GLfloat nx, ny, nz;  // Normal vector
        };

        bool m_renderBB = false;
        AABB m_bounds;


        std::vector<CubeVertex> normalVertices;
        std::vector<unsigned short> cubeIndices;

        // Render needs
        wolf::Program* m_pProgram = nullptr;
        wolf::VertexBuffer* m_pVB = nullptr;
        wolf::IndexBuffer* m_pIB = nullptr;
        wolf::VertexDeclaration* m_pDecl = nullptr;

        wolf::Texture* m_pTexture = nullptr;


};