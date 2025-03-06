#pragma once

#include "../../wolf/wolf.h"

#include <stdio.h>
#include <iostream>
#include <glm/glm.hpp>

#include "Quad.h"
#include "ProcessFile.h"
#include "TextTable.h"
#include "TextBox.h"



//Helper for the runner
int RunTextRenderingDemo();

class TextRenderingDriver : public wolf::App {
    public:
        // Constructor
        TextRenderingDriver(const std::string& name);
        // Deconstructor
        ~TextRenderingDriver();

        /*
        // Creates a Instance of our scene
        // Init sets up the Main FPS camera
        // Builds the OctTree
        */
        void Init();

        /*
        // Updates all the Nodes within the Scene Instance
        // This method is also overrides a call from wolf and is called every frame
        // In the parameter we have a float which provides the delta of the last and recent frame
        */
        void update(float p_fDelta) override;

        /*
        // Renders all the Nodes within the Scene Instance
        // This method overrides from a call in wolf which is called each frame
        */
        void render() override;



    private:

        // Methods
        void Sample1();
        void TextBoxRightAlign();
        void TextBoxCenterAlign();
        void TextBoxLeftAlign();
        void SetupTextTable();

        TextBox* pTextBox;
        TextBox* pTextBox2;
        TextBox* pTextBox3;
    
        TextTable* table;
};