#pragma once

#include "../wolf/wolf.h"
#include <string>

#include "nodeClass/scene.h"    //A singleton which stores the instance of the scene

#include "nodeClass/node.h"
#include "nodeClass/debugCube.h"
#include "particleSystem/billBoard.h"

int RunMainProject();

class ProjectRunner : public wolf::App{
    public:
        // Constructor
        ProjectRunner(const std::string &name);

        ~ProjectRunner();


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

        // Build a basic Enviroment for testing purposes
        void BuildBasicEnviroment();


    private:

        Camera* m_pMainCamera;  

};