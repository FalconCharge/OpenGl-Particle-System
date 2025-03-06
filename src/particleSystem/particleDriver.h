#pragma once

#include "../../wolf/wolf.h"
#include <string>

#include "../nodeClass/scene.h"    //A singleton which stores the instance of the scene

#include "../nodeClass/node.h"
#include "../nodeClass/debugCube.h"
#include "billBoard.h"
#include "pointBB.h"
#include "effect.h"
#include "boxEmitter.h"

#include "circleShapeParticle.h"

//Affectors
#include "movementAffectors.h"
#include "gravityAffector.h"
#include "lifeTimeAffector.h"
#include "scaleAffector.h"
#include "rotateAffector.h"

#include "../textRendering/src/TextBox.h"

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
        void render() override;



    private:
        // Starts the build for the demo enviroment
        void BuildDemoEnviroment();
        // Create an emitter
        void CreateEffect();
        // Builds a Smoke Object
        void BuildSmoke();

        Camera* m_pMainCamera;
        TextBox* m_pFPSTextBox;
};