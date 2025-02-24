#pragma once

#include "scene.h"

class OctTree;

class OctTreeNode {
    public:
        OctTreeNode(AABB* bounds, int maxDepth, OctTree* tree);
        OctTreeNode(){}
        ~OctTreeNode();

        //
        void BuildTree(int nodesPerOctant);

        //Getters
        bool IsLeaf(){return m_IsLeaf;}
        AABB* GetBounds(){return m_bounds;}
        std::vector<OctTreeNode*> GetChildren(){return m_children;}
        std::vector<AABB*> GetChildBounds(){return m_childBounds;}

        // Subdivides the Octant
        void Subdivide();
        // Inserts a node into the Octants
        void Insert(Node* object);

        // Grabs all the nodes within the frustum and adds them to visibleNodes
        void OctTreeNode::QueryVisibleNodes(Frustum& frustum, std::vector<Node*>& visibleNodes);


    private:
        bool m_IsLeaf = true;
        int m_depth = 0;

        AABB* m_bounds;
        int m_maxDepth;

        // A list of objects in this node
        std::vector<Node*> m_objects;

        std::vector<OctTreeNode*> m_children;
        std::vector<AABB*> m_childBounds;

        // Include a reference to the Tree itself
        OctTree* m_octTree = nullptr;
};