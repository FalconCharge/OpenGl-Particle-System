#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include "../../wolf/wolf.h"
#include <iostream>

//Vertex information
struct Vertex{
    GLfloat x, y;
    GLfloat u, v;
};

//A class that stores the informatin of the values of the quad and is also able to use render
class Quad {
    public:
        Quad();
        ~Quad();

        void Init();
        void render(int width, int height);

        //Setters Methods
        void setPosition(glm::vec2 position){m_vPosition = position; updateVertices();}
        void setScale(glm::vec2 scale);
        void setRotation(float angle) { m_fRotation = angle; updateVertices();} 
        void SetTexture(wolf::Texture* tex) {m_texture = tex;};
        void SetTextureCoordinates(GLfloat u1, GLfloat v1, GLfloat u2, GLfloat v2);
        void SetName(char name){m_cName = name;}
        void setColor(glm::vec4 color){m_vColor = color;}
        void setShader(wolf::Program* program) { m_pProgram = program; }
        void SetTextureID(int textureID){m_textureLocation = textureID;};


        //Getters
        glm::vec2 getPosition(){return m_vPosition;}
        glm::vec2 getScale(){return m_vScale;}
        float getRotation(){return m_fRotation; }
        glm::vec4 getColor(){ return m_vColor; }
        bool isDirty(){return m_isDirty;}
        std::vector<Vertex> GetVertices();
        int getTextureID(){return m_textureLocation;}


        //Debud method for quad information
        void DebugInfo() {
            std::cout << "Quad Debug Info " << m_cName << ":\n"
                    << "Position: (" << m_vPosition.x << ", " << m_vPosition.y << ")\n"
                    << "Scale: (" << m_vScale.x << ", " << m_vScale.y << ")\n"
                    << "Rotation: " << m_fRotation << " degrees\n"
                    << "Color: (" << m_vColor.r<<", " << m_vColor.g <<", "<< m_vColor.b << ")\n"
                    << "Texture Coordinates: (" << m_u1 << ", " << m_v1 << ") - (" << m_u2 << ", " << m_v2 << ")\n"
                    << "Texture: " << (m_texture ? "Set" : "None") << "\n"
                    << std::endl;
        }


    private:
        //Basic values
        char m_cName;               //Used to Identify the quad
        glm::vec2 m_vPosition;      //The position of the quad
        glm::vec2 m_vScale = glm::vec2(5.0f);   //Scale of the quad
        float m_fRotation = 0.0f;               //How much rotation is applied to the quad (Not tested)
        glm::vec4 m_vColor;                     //Sets the color of the quad


        bool m_isDirty = false;     //Used to tell if the quad values need to be recaculated
        int m_textureLocation;      //What page value the texture is on

        // UV values with default
        GLfloat m_u1 = 0.0f, m_v1 = 0.0f, m_u2 = 1.0f, m_v2 = 1.0f;


        //Basic render needs
        wolf::VertexBuffer* m_VBquad = 0;
        wolf::VertexDeclaration* m_pDecl = 0;
        wolf::Program* m_pProgram = 0;
        wolf::Texture* m_texture;

        //Vertices and generation
        void generateVertices();
        void updateVertices();
        std::vector<Vertex> vertices;   //A base vertices to return to
        std::vector<Vertex> m_newVerts; //Used to update vertices

};

