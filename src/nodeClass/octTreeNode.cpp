#include "octTreeNode.h"
#include "octTree.h"


OctTreeNode::OctTreeNode(AABB* bounds, int depth, OctTree* tree){
    m_octTree = tree;    // Ref to the main octtree 
    m_octTree->AddBound(bounds);
    m_IsLeaf = true;
    m_bounds = bounds;
    m_children.resize(8, nullptr);  //init to null values
    m_childBounds.resize(8, nullptr);    
    m_depth = depth;
    m_maxDepth = tree->GetMaxDepth();

}
OctTreeNode::~OctTreeNode() {
    // Delete the bounds if it is owned by this node
    if (m_bounds) {
        delete m_bounds;
        m_bounds = nullptr;
    }

    // Delete all child nodes
    for (OctTreeNode* child : m_children) {
        if (child) {
            delete child;
        }
    }
    m_children.clear();

    // Delete all child bounds
    for (AABB* bounds : m_childBounds) {
        if (bounds) {
            delete bounds;
        }
    }
    
    m_childBounds.clear();
    m_objects.clear();
}
void OctTreeNode::Subdivide() {
    if(!m_IsLeaf){return;}  //already divided

    m_IsLeaf = false; // Since we're subdividing, this node is no longer a leaf

    glm::vec3 parentCenter = m_bounds->GetCenter();
    glm::vec3 parentMin = m_bounds->GetMin();
    glm::vec3 parentMax = m_bounds->GetMax();
    glm::vec3 childSize = (parentMax - parentMin) * 0.5f;




    for (int i = 0; i < 8; i++) {

        // Compute child min/max based on which quadrant of the parent it occupies
        glm::vec3 childMin = parentMin;
        glm::vec3 childMax = parentCenter;

        if (i & 1)        { childMin.x = parentCenter.x; childMax.x = parentMax.x; }
        if ((i & 2) >> 1) { childMin.y = parentCenter.y; childMax.y = parentMax.y; }
        if ((i & 4) >> 2) { childMin.z = parentCenter.z; childMax.z = parentMax.z; }

        // Compute the child's new center
        glm::vec3 childCenter = (childMin + childMax) * 0.5f;

        glm::vec3 childDimensions = (childMax - childMin);
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), childCenter) *
                              glm::scale(glm::mat4(1.0f), childDimensions);
        

        // Create a new AABB for the child node
        m_childBounds[i] = new AABB(transform);
        m_childBounds[i]->SetColor(m_octTree->GetColors()[i]);
        
    }

    //Move nodes into the children
    for(auto it = m_objects.begin(); it != m_objects.end();){
        Node* obj = *it;
        bool inserted = false;

        for(int i = 0; i < 8; i++){
            if(m_childBounds[i]->Contains(obj->GetVolume().GetMin(), obj->GetVolume().GetMax())) {
                if(!m_children[i]){
                    m_children[i] = new OctTreeNode(m_childBounds[i], m_depth + 1, m_octTree);
                }
                m_children[i]->Insert(obj);
                inserted = true;
            }
        }
        if(inserted){
            it = m_objects.erase(it);
        }else{
            ++it;
        }
    }
    m_objects.clear();
}

void OctTreeNode::Insert(Node* object){

    if(m_IsLeaf){
        m_objects.push_back(object);
        return;
    }

    bool inserted = false;
    for(int i = 0; i < 8; i++){
        if (m_childBounds[i] && m_childBounds[i]->Contains(object->GetVolume().GetMin(), object->GetVolume().GetMax())) {
            if (!m_children[i]) {
                m_children[i] = new OctTreeNode(m_childBounds[i], m_depth + 1, m_octTree);
            }
            m_children[i]->Insert(object);
            inserted = true;
        }
    }
    if(!inserted){
        m_objects.push_back(object);
    }
}

void OctTreeNode::BuildTree(int nodesPerOctant){
    if(m_IsLeaf && m_objects.size() > nodesPerOctant && m_depth < m_maxDepth){
        Subdivide();
    }
    if(!m_IsLeaf){
        for(int i = 0; i < 8; i++){
            if(m_children[i]){
                m_children[i]->BuildTree(nodesPerOctant);
            }
        }
    }
}

void OctTreeNode::QueryVisibleNodes(Frustum& camFrustum, std::vector<Node*>& visibleNodes) {
    if (!m_bounds->IsOnFrustum(camFrustum)) {
        return;  // Skip this node if it's completely outside the frustum
    }

    // If it's a leaf node, add all objects inside
    if (m_IsLeaf) {
        for(Node* obj : m_objects){
            // Checks if the Node is in the frustum
            if(obj->IsVisible()){
                visibleNodes.push_back(obj);
            }
        }
        return;
    }

    // Otherwise, check children
    for (int i = 0; i < 8; i++) {
        if (m_children[i]) {
            m_children[i]->QueryVisibleNodes(camFrustum, visibleNodes);
        }
    }
}



