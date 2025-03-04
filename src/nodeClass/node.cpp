#include "node.h"

Node::Node(){
    //Default coloring scheme
    this->SetColor(glm::vec4(1, 1, 1, 1));
}
Node::~Node(){}
Node::Node(std::string name) : m_name(name){}


void Node::AddChild(Node* newChild){
    newChild->SetParent(this);
    m_isDirty = true;


    m_children.push_back(newChild);

    //Default coloring

}

void Node::SetParent(Node* parent){
    m_parent = parent;
}
glm::vec3 Node::GetWorldPosition() {
    if(this->getParent() != nullptr){
        return this->GetLocalPosition() + this->getParent()->GetWorldPosition();
    }
    return this->GetLocalPosition();
}
glm::vec3 Node::GetWorldScale(){
    if(this->getParent() != nullptr){
        return this->getLocalScale() * this->getParent()->GetWorldScale();     //Difference is the *
    }
    return this->getLocalScale();
}
glm::vec3 Node::GetWorldRotation(){
    if(this->getParent() != nullptr){
        return this->getLocalRotation() + this->getParent()->GetWorldRotation();
    }
    return this->getLocalRotation();
}

glm::mat4 Node::GetWorldTransform() {

    if (m_parent != nullptr) {
        // Combine parent's world transform with this node's local transform
        return m_parent->GetWorldTransform() * GetLocalTransform();
    }
    return GetLocalTransform();
}
glm::mat4 Node::GetLocalTransform(){
    if(m_isDirty){
        m_transform = glm::mat4(1.0f);

        // Apply translation
        m_transform = glm::translate(m_transform, m_position);

        // Apply rotation
        m_transform = glm::rotate(m_transform, glm::radians(m_rotation.x), glm::vec3(1, 0, 0)); // Rotate around X
        m_transform = glm::rotate(m_transform, glm::radians(m_rotation.y), glm::vec3(0, 1, 0)); // Rotate around Y
        m_transform = glm::rotate(m_transform, glm::radians(m_rotation.z), glm::vec3(0, 0, 1)); // Rotate around Z

        // Apply scaling
        m_transform = glm::scale(m_transform, m_scale);

        m_isDirty = false;
    }
    return m_transform;
}
void Node::setWorldScale(glm::vec3 scale) {
    if (m_parent != nullptr) {
        // Calculate the local scale relative to the parent
        m_scale = scale / m_parent->GetWorldScale(); // Divide by parent's world scale
    } else {
        m_scale = scale; // No parent, set the world scale directly as local scale
    }
    m_isDirty = true;
}
void Node::SetWorldPosition(glm::vec3 position) {
    if (m_parent != nullptr) {
        // Adjust the position relative to the parent’s world position
        m_position = position - m_parent->GetWorldPosition();
    } else {
        // If there is no parent, just set the position
        m_position = position;
    }

    m_isDirty = true;  // Mark the node as dirty
}
void Node::setWorldRotation(glm::vec3 rotation) {
    if (m_parent != nullptr) {
        // Convert the world rotation into local rotation relative to the parent
        m_rotation = rotation - m_parent->GetWorldRotation(); // Subtract parent's world rotation
    } else {
        m_rotation = rotation; // No parent, set the world rotation directly as local rotation
    }
    m_isDirty = true;
}

void Node::Debug(){
    std::cout << "Node Name: " << m_name << "\n"
              << "Position: (" << m_position.x << ", " << m_position.y << ", " << m_position.z << ")\n"
              << "Scale: (" << m_scale.x << ", " << m_scale.y << ", " << m_scale.z << ")\n"
              << "Rotation: (" << m_rotation.x << ", " << m_rotation.y << ", " << m_rotation.z << ")\n"
              << "World Position: (" << GetWorldPosition().x << ", " << GetWorldPosition().y << ", " << GetWorldPosition().z << ")\n"
              << std::endl;
}

void Node::Update(float fDelta){
    // Call the update on the children
    for (Node* child : m_children)
    {
        child->Update(fDelta);
    }
}
void Node::Render(glm::mat4 proj, glm::mat4 view){
    //Don't need to render the children since they will be added to a list of nodes to render
    
}

bool Node::IsVisible() {
    if (m_pVolume == nullptr) {
        std::cout << "[DEBUG] Volume is null for node: " << m_name << std::endl;
        return true;
    }
    //static_cast<AABB*>(GetVolume())->Debug();
    // Retrieve the camera's frustum
    Frustum& frustum = Scene::Instance().GetActiveCamera()->GetFrustum();

    // If the node has a volume, use it to check frustum culling
    bool result = m_pVolume->IsOnFrustum(frustum);

    //std::cout << "[DEBUG] Node " << m_name << " is " << (result ? "VISIBLE" : "NOT visible") << std::endl;

    return result;
}



void Node::CollectVisibleNodes(std::vector<Node*>& visibleNodes) {
    if (IsVisible()) {
        visibleNodes.push_back(this);
    }
    // Recursively check all child nodes
    for (Node* child : m_children) {
        child->CollectVisibleNodes(visibleNodes);
    }
}

glm::vec3 Node::GetGlobalScale(){
    // The first three columns of the matrix represent the local axes,
    // whose lengths correspond to the scaling factors.
    glm::vec3 scale;
    scale.x = glm::length(glm::vec3(GetWorldTransform()[0])); // Column 0
    scale.y = glm::length(glm::vec3(GetWorldTransform()[1])); // Column 1
    scale.z = glm::length(glm::vec3(GetWorldTransform()[2])); // Column 2
    return scale;
}
bool Node::IsEqual(Node* other){
    if(this->GetWorldPosition() != other->GetWorldPosition()){return false;}
    if(this->GetWorldScale() != other->GetWorldScale()){return false;}
    if(this->GetWorldRotation() != other->GetWorldRotation()){return false;}
    if(this->GetName() != other->GetName()){return true;}
    //Color to
    return true;

}

