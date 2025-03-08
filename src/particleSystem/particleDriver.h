//-----------------------------------------------------------------------------
// File:            particleDriver.h
// Original Author: Ben Harper
// Date:            March.8/2025
//
// Description:
// ParticleDriver is the main application class for the particle system demo. 
// It handles initialization, rendering, and updating of the particle system, 
// along with managing the scene, camera, and UI elements.
//
//-----------------------------------------------------------------------------  



#pragma once

#include "../../wolf/wolf.h"

#include "../nodeClass/scene.h"    //A singleton which stores the instance of the scene

#include "../nodeClass/debugCube.h"     // Ground
#include "../textRendering/src/TextBox.h"
#include "effect.h"
#include "particleSystemFactory.h"

#include <string>


//Helper for the runner
int RunParticleSystemDemo();

class ParticleDriver : public wolf::App {
    public:
        // Constructor
        ParticleDriver(const std::string& name);
        // Deconstructor
        ~ParticleDriver();

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
        void render();



    private:
        // Loads an effect from a certain file
        void LoadEffect(const std::string& filename);

        void ReloadParticleSystem();

        void BuildDemoEnviroment();

        void SetupCamera();

        void SetUpTextBox();

        void UpdateEffect();




        Camera* m_pMainCamera = nullptr;
        TextBox* m_pFPSTextBox = nullptr;
        // Current Effect Should Prob change this to spawn several in
        Effect* m_pEffect = nullptr;

        std::vector<std::string> m_effectFileNames;

        int m_index = 0;

};