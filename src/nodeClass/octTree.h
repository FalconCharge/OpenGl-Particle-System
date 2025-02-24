#pragma once
#include "scene.h"
#include "octTreeNode.h"

class Node;
class AABB;

class OctTree{
    public:
        // Consturctor
        OctTree(std::vector<Node*> worldObjects, int maxDepth, int nodesPerOctant);
        ~OctTree();

        std::vector<const AABB*> GetBounds(){return m_allBounds;}
        void AddBound(const AABB* aabb){m_allBounds.push_back(aabb);}
        int GetMaxDepth(){return m_maxDepth;}

        void QueryVisibleNodes(Frustum& frustum, std::vector<Node*>& nodes);
        std::vector<glm::vec3> GetColors(){return m_childColors;}
    
    private:
        
        void CalculateBounds(std::vector<Node*> worldObjects);

        AABB* m_bound;
        OctTreeNode* m_root;
        int m_maxDepth; //Used to stop infinite recursion

        // For rendering
        std::vector<const AABB*> m_allBounds = {};

        std::vector<glm::vec3> m_childColors;

        float m_minNodeSize;

        

};
