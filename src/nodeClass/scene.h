#pragma once
#include "camera.h"
#include <vector>   // For node storage
#include "glm/glm.hpp"
#include "node.h"
#include "aabb.h"

class Node; // Forward declartion becuase Node includes scene.h
class OctTree; // Forward declaration of the OctTree class


class Scene{
    public:
        static Scene& Instance();

        // Add a top level node to the scene
        void AddNode(Node* node);

        void Update(float fDelta);
        void Render();
        void BuildOctTree();

        void Clear();

        // Setters
        void SetActiveCamera(Camera* pCam);

        // Getters
        Camera* GetActiveCamera(){return m_pActiveCamera;}
        const std::vector<Node*>& GetNodes();
        void RenderOctTree();
        void RenderCubes();


    private:
        
        Scene() {}
        ~Scene() {}

        // prevent copying
        Scene(const Scene&) = delete;
        Scene& operator=(const Scene&) = delete;

        void CollectAllNodes(std::vector<Node*>& allNodes, Node* node);

        Camera* m_pActiveCamera = nullptr;

        // Performs culling by asking each top-level node to add itself and its visible children
        // to the list of objects to render.
        std::vector<Node*> DoCulling();

        // List of top-level nodes.
        std::vector<Node*> m_nodes;
        // List of nodes determined to be visible after culling.
        std::vector<Node*> m_objectsToRender;



        OctTree* m_pOctTree = nullptr;
        bool m_bRenderOctTree = false;
        bool m_bRenderCubes = true;
};

