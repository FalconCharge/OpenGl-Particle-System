#include "octTree.h"

#include <glm/gtx/string_cast.hpp>  // For glm::to_string



// Need to make a deconstructor for this

OctTree::OctTree(std::vector<Node*> worldObjects,  int maxDepth = 20, int nodesPerOctant = 2){


    //Basic safe checks
    if (maxDepth <= 0 || maxDepth > 30) {
        maxDepth = 10; 
    }
    if (nodesPerOctant <= 0) {
        nodesPerOctant = 5; 
    }

    // Init the colors
    m_childColors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
    m_childColors.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
    m_childColors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
    m_childColors.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
    m_childColors.push_back(glm::vec3(1.0f, 0.0f, 1.0f));
    m_childColors.push_back(glm::vec3(0.0f, 1.0f, 1.0f));
    m_childColors.push_back(glm::vec3(1.0f, 0.5f, 0.0f));
    m_childColors.push_back(glm::vec3(0.5f, 0.0f, 1.0f));

    m_maxDepth = maxDepth;

    CalculateBounds(worldObjects);
    m_root = new OctTreeNode(m_bound, 0, this);

    //Insert the nodes into the tree
    for(Node* obj : worldObjects){
        m_root->Insert(obj);
    }

    m_root->BuildTree(nodesPerOctant);

}
// Destructor
OctTree::~OctTree() {
    if (m_bound) {
        delete m_bound;
        m_bound = nullptr;
    }

    if (m_root) {
        delete m_root;
        m_root = nullptr;
    }

    m_allBounds.clear();
}

void OctTree::CalculateBounds(std::vector<Node*> worldObjects) {
    if (worldObjects.empty()) {
        std::cerr << "No objects in the scene, cannot calculate bounds!" << std::endl;
        return;
    }

    
    // Start with extreme values
    glm::vec3 minPoint(FLT_MAX);
    glm::vec3 maxPoint(-FLT_MAX);

    for (Node* node : worldObjects) {
        AABB& aabb = node->GetVolume();

        // Compute the node's min and max in world space
        glm::vec3 nodeMin = aabb.GetMin();
        glm::vec3 nodeMax = aabb.GetMax();

        // Expand the overall bounds
        minPoint = glm::min(minPoint, nodeMin);
        maxPoint = glm::max(maxPoint, nodeMax);
        
    }

    // Compute the center and uniform extent of the bounding box that encompasses all nodes.
    glm::vec3 center = (minPoint + maxPoint) * glm::vec3(0.5);

    glm::vec3 halfSizes = (maxPoint - minPoint);
    float extent = std::max(halfSizes.x, std::max(halfSizes.y, halfSizes.z));
    

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), center) * glm::scale(glm::mat4(1.0f), glm::vec3(extent));

    // Extract the translation (center) from the transform
    glm::vec3 extractedCenter = glm::vec3(transform[3]);

    // Since we're using a uniform scale, extract the scale factor from one axis vector
    glm::vec3 scaleVec(glm::length(glm::vec3(transform[0])),
                        glm::length(glm::vec3(transform[1])),
                        glm::length(glm::vec3(transform[2])));
    float newScale = glm::max(scaleVec.x, glm::max(scaleVec.y, scaleVec.z)); // Use max scale

    float extents = newScale * 0.5f;

    // Create the final AABB
    AABB* fAABB = new AABB(transform);
    // Goes to the will transform to the global center value on render and adjust it's extent
    
    fAABB->SetColor(m_childColors[0]);  //Set to RED
    m_bound = fAABB;    // Personal bound  

      
}


void OctTree::QueryVisibleNodes(Frustum& frustum, std::vector<Node*>& visibleNodes){
    if(m_root){
        m_root->QueryVisibleNodes(frustum, visibleNodes);
    }
}




