#pragma once

#include "../nodeClass/node.h"
#include "../../wolf/wolf.h"
#include "emitter.h"

/*
//  Top Level which is a container for emitters which is derived from Node (For the position attribute)
//  It will be incharge of calling updates Like Play, Stop, Pause for the emitters
//  It will also call Update and Render for the emitters
*/
class Effect : public Node{
    public:
        Effect();
        ~Effect();

        void Init();
        void Update(float p_fDelta) override;
        void Render(glm::mat4 p_view, glm::mat4 p_Proj) override;
        void Play();
        void Stop();
        void Pause();

        AABB& CalculateVolume();
        void AddEmitter(Emitter* e);

    private:
        std::vector<Emitter*> m_pEmitters;
        AABB m_bounds;


        // For Rendering
        void SetupRendering();
        void FlushVB(wolf::Material* currentMaterial, const std::vector<Point>& vertices);
        wolf::VertexDeclaration* m_pDecl = nullptr; // Declartion
        wolf::VertexBuffer* m_pVB = nullptr;        // Vertex buffer
        wolf::Texture* m_pTexture = nullptr;        // Texture of the point

};