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
#include "../tinyxml/tinyxml.h"

#include "circleShapeParticle.h"

//Affectors
#include "movementAffectors.h"
#include "gravityAffector.h"
#include "lifeTimeAffector.h"
#include "scaleAffector.h"
#include "rotateAffector.h"

#include "../textRendering/src/TextBox.h"

#include "particleSystemFactory.h"
#include "../utility/Unility.h"

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
        // Load an effect from file
        void LoadEffect(const std::string& filename);

        // Create an effect from file
        Effect* CreateEffectFromFile(const std::string& filename);
        // Load an emitter from file
        void LoadEmitterFromFile(const std::string& emitterFile, Emitter* emitter);
        // Reload the particle system
        void ReloadParticleSystem();

        // Starts the build for the demo enviroment
        void BuildDemoEnviroment();
        // Create an emitter
        void CreateEffect();
        // Builds a Smoke Object
        void BuildSmoke();
        // Creats an Effect from file

        Camera* m_pMainCamera = nullptr;
        TextBox* m_pFPSTextBox = nullptr;
        // Current Effect Should Prob change this to spawn several in
        Effect* m_pEffect = nullptr;

        // Map to store emitters by their file paths
        std::unordered_map<std::string, Emitter*> m_emitters;
};