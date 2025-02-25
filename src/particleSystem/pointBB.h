#pragma once

#include "../../wolf/wolf.h"
#include "../nodeClass/scene.h"

struct PointBBVertex{
    GLfloat x,y,z;       // Position Cords
};

class PointBB : public Node{

    public:
        // Constructor
        PointBB();
        // constructor with poisitonal parameters
        PointBB(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation);
        PointBB(glm::vec3 position);

        // Deconstructor
        ~PointBB();

        // Initlizes the values for the BB
        void Init();
        // Renders the BB
        void Render(glm::mat4 proj, glm::mat4 view) override;
        // Performs Updates each frame on the BB
        void Update(float p_fDelta) override;

        // Creates a new Texture within the BB
        void SetTexture(const std::string& texturePath);
        // Sets a new Texture to this BB
        void SetTexture(wolf::Texture* p_pTexture){m_pTexture = p_pTexture;}
        // Creates a new shader within the BB
        void SetShader(const std::string& vshPath, const std::string& fshPath);
        // Sets a new shader to this BB
        void SetShader(wolf::Program* p_pShader){m_pProgram = p_pShader;};

        // Returns a AABB that surrounding this object
        AABB& CalculateVolume() override;

        // Updates the VB data Used If we have a new shader or texture to apply to the BillBoard
        void UpdateVerts();

        void SetVertices(std::vector<PointBBVertex> normalVertices);


    private:

        void FaceCamera();


        //wolf 
        wolf::Program* m_pProgram = nullptr;
        wolf::VertexBuffer* m_pVB = nullptr;
        //wolf::IndexBuffer* m_pIB = nullptr;
        wolf::Texture* m_pTexture = nullptr;
        wolf::VertexDeclaration* m_pDecl = nullptr;

        std::vector<PointBBVertex> normalVertices;
        //std::vector<unsigned short> cubeIndices;

        //The bounds of the BillBoard
        AABB m_bounds;



};