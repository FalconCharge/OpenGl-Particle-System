#pragma once
#include <glm/glm.hpp>
#include "../../wolf/wolf.h"
#include <iostream>
#include <string>
#include <vector>
#include "scene.h"
#include "aabb.h"

class AABB;

class Node{
    public:
        //Constructor
        Node();
        Node(std::string name);

        virtual ~Node();
        virtual void Update(float fDelta);
        virtual void Render(glm::mat4 proj, glm::mat4 view);

        //Used to collect the visible nodes
        virtual void CollectVisibleNodes(std::vector<Node*>& visibleNodes);
        virtual AABB& CalculateVolume() = 0;

        // Adds a child to the node
        void AddChild(Node* newChild);
        void RemoveChild(Node* child);

        // Setters      
        void SetName(const std::string newName){m_name = newName;}                              
        void setTransform(glm::mat4 newMatrix){m_transform = newMatrix;}            
        void SetParent(Node* newParent);          
        void SetColor(glm::vec4 color){m_color = color;}
        void SetVolume(AABB& vol){m_pVolume = &vol;}
        void SetDirty(bool dirty){m_isDirty = dirty;}
        

        // Getters
        std::string GetName()const {return m_name;}             // Returns a const of the name of the node
        Node* getParent(){return m_parent;}                     // Returns the pointer to the parent Node
        std::vector<Node*> getChildren(){return m_children;}    // Returns the list of children R/O
        glm::vec4 getColor(){return m_color;}
        bool getDirty(){return m_isDirty;}
        AABB& GetVolume(){return *m_pVolume;}
        std::vector<Node*> GetChildren(){return m_children;}

        glm::vec3 GetWorldPosition();
        glm::vec3 GetWorldScale();
        glm::vec3 GetGlobalScale();
        glm::vec3 GetWorldRotation();
        glm::mat4 GetWorldTransform();
        
        glm::vec3 getLocalScale(){return m_scale;}
        glm::vec3 GetLocalPosition(){return m_position;}
        glm::vec3 getLocalRotation(){return m_rotation;}
        glm::mat4 GetLocalTransform();

        //Setters
        void SetLocalScale(glm::vec3 scale){m_scale = scale; m_isDirty = true;}
        void SetLocalPosition(glm::vec3 position){m_position = position; m_isDirty = true;}
        void SetLocalRotation(glm::vec3 rotation){m_rotation = rotation; m_isDirty = true;}

        void setWorldScale(glm::vec3 scale);
        void SetWorldPosition(glm::vec3 position);
        void setWorldRotation(glm::vec3 rotation);

        void Scale(glm::vec3 scale){m_scale *= scale; m_isDirty = true;}
        void Rotate(glm::vec3 rotate){m_rotation += rotate; m_isDirty = true;}
        void Translate(glm::vec3 position){m_position += position; m_isDirty = true;}

        virtual bool IsVisible();   
        void Debug();

        bool IsEqual(Node* other);

    private:

        glm::vec4 m_color;
        glm::vec3 m_scale;
        glm::vec3 m_position;
        glm::vec3 m_rotation;
        glm::mat4 m_transform;

        bool m_isDirty;

        
        std::string m_name;
        Node* m_parent = nullptr;
        std::vector<Node*> m_children;

        AABB* m_pVolume = nullptr;

};