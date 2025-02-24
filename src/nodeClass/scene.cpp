#include "scene.h"
#include "octTree.h"

#include <unordered_map>
#include <set>

// Returns the singleton instance of Scene.
Scene& Scene::Instance() {
    static Scene instance;
    return instance;
}
void Scene::Clear() {
    // Delete all nodes
    
    m_nodes.clear();
    m_objectsToRender.clear();

}
void Scene::RenderOctTree(){
    m_bRenderOctTree = !m_bRenderOctTree;
}
void Scene::RenderCubes(){
    m_bRenderCubes = !m_bRenderCubes;
}

// Adds a top-level node to the scene.
void Scene::AddNode(Node* node) {
    m_nodes.push_back(node);
}

// Returns the list of top-level nodes.
// Note: This function is declared as const, so we must also mark it as const.
const std::vector<Node*>& Scene::GetNodes(){
    return m_nodes;
}

std::vector<Node*> Scene::DoCulling(){

    std::vector<Node*> visibleNodesOctTree;

    if(m_pOctTree){
        m_pOctTree->QueryVisibleNodes(GetActiveCamera()->GetFrustum(), visibleNodesOctTree);

        //std::cout << "Amount of cubes in Octant's being rendered: " << visibleNodesOctTree.size() << std::endl;

        std::set<Node*> uniqueNodes(visibleNodesOctTree.begin(), visibleNodesOctTree.end());

        std::vector<Node*> uniqueVisibleNodes(uniqueNodes.begin(), uniqueNodes.end());

        //std::cout << "Amount of unique Nodes being rendered: " << uniqueVisibleNodes.size() << std::endl;

        return uniqueVisibleNodes;
    }else{
        for(Node* node : m_nodes){
            node->CollectVisibleNodes(visibleNodesOctTree);
        }
        //std::cout << "Amount of cubes being rendered no octtree: " << visibleNodesOctTree.size() << std::endl;
        return visibleNodesOctTree;
        
    }


        
}
void Scene::SetActiveCamera(Camera* pCam){
    m_pActiveCamera = pCam;
}

void Scene::Render(){
    // Clear the screen
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glEnable(GL_CULL_FACE); //Culls the faces on the inside of nodes


    glm::mat4 proj = GetActiveCamera()->getProjMatrix();
    glm::mat4 view = GetActiveCamera()->getViewMatrix();

    //Rendering the OctTree
    if (m_pOctTree != nullptr) { 
        if(m_bRenderOctTree){
            std::vector<const AABB*> bounds = m_pOctTree->GetBounds();

            for(const AABB* aabb : bounds){

                aabb->Render(view, proj);
            }
        }
    }
    if(m_bRenderCubes){
        for(Node* node : m_objectsToRender){
            node->Render(proj, view);
        }
    }
}
void Scene::Update(float fDelta){    
    this->GetActiveCamera()->update(fDelta);

    for(Node* node : m_nodes){
        node->Update(fDelta);
    }

    // Redo the culling each time a new scene is made
    m_objectsToRender = DoCulling();
}
// Pushes children into a single list
void Scene::CollectAllNodes(std::vector<Node*>& allNodes, Node* node) {
    // Add the current node to the list
    allNodes.push_back(node);

    // If this node has children, recursively collect all child nodes
    for (Node* child : node->GetChildren()) {
        CollectAllNodes(allNodes, child);
    }
}


// Creates a OctTree and fills in the objects
void Scene::BuildOctTree(){

    std::vector<Node*> allNodes;

    for(Node* node : m_nodes){
        CollectAllNodes(allNodes, node);
    }

    m_pOctTree = new OctTree(allNodes, 50, 3);
}