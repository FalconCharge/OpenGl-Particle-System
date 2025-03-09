#pragma once

#include "../nodeClass/node.h"
#include "../../wolf/wolf.h"
#include "emitter.h"
#include <unordered_map>

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

        // Not implemented
        void Play();
        void Stop();
        void Pause();
        // Not implemented

        // Needed for a Noode (Just makes a huge node so it always renders)
        AABB& CalculateVolume();
        // Adds an emitter this effect
        void AddEmitter(Emitter* e);

    private:
        // A list of the effects emitters
        std::vector<Emitter*> m_pEmitters;
        
        AABB m_bounds;

        // Grabs all the points; sorts em, and turns them into Points to be rendered in proper order
        void GetVertexData();
        std::vector<Point> m_pPoints;

        std::vector<Point> SortPoints(std::vector<PointBB*>);

        void Effect::RadixSort(std::vector<PointBB*>& pointBBS);
        void Effect::CountSort(std::vector<PointBB*>& pointBBS, GLfloat exp);


        // For Rendering
        void SetupRendering();
        void FlushVB(wolf::Material* currentMaterial, std::vector<Point>& vertices);

        wolf::VertexDeclaration* m_pDecl = nullptr; // Declartion
        wolf::VertexBuffer* m_pVB = nullptr;        // Vertex buffer

};