//-----------------------------------------------------------------------------
// File:            particleDriver.cpp
// Original Author: Ben Harper
// Date:            March.8/2025
//
// Description:
// Implementation of the ParticleDriver class, responsible for running the 
// particle system demo, managing effects, handling input, and updating 
// scene objects.
//-----------------------------------------------------------------------------

#include "particleDriver.h"
#include <iostream>
#include <chrono>

int RunParticleSystemDemo(){
    ParticleDriver app("Particle System Demo");
    app.run();
    return 0;
}

ParticleDriver::ParticleDriver(const std::string& name) : wolf::App(name){
    std::cout << "Starting up particle system demo" << std::endl;
    Init();
}
ParticleDriver::~ParticleDriver(){
    delete m_pEffect;
    delete m_pMainCamera;
    delete m_pFPSTextBox;
}
void ParticleDriver::Init(){
    glEnable(GL_PROGRAM_POINT_SIZE);    // Used for rendering Points (This would be something that should be on the material)
    SetUpTextBox();
    // Builds the tree for the scene
    //Scene::Instance().BuildOctTree();  Ben personally doesn't want to do this right now: Feb.24
    SetupCamera();
    BuildDemoEnviroment();
}
void ParticleDriver::update(float p_fDelta){
    //Updates all the Nodes that were added to the scene instance
    Scene::Instance().Update(p_fDelta);
    // Updates the FPS Text
    m_pFPSTextBox->setDisplayText(std::to_string(int(this->getFPS())));

    ReloadParticleSystem();
    UpdateEffect();
}
void ParticleDriver::render(){
    // Renders all the Nodes that were added the the scene instance
    Scene::Instance().Render();

    m_pFPSTextBox->render(m_width, m_height);
}
void ParticleDriver::BuildDemoEnviroment(){
    
    DebugCube *pCube1 = new DebugCube(glm::vec3(0, 0, 0), glm::vec3(10, 0.25, 10), glm::vec3(0));
    pCube1->SetName("Ground");
    pCube1->SetShader("Data/Shader/groundShader.vsh", "Data/Shader/groundShader.fsh");  //Needs to be called before init
    pCube1->Init();
    pCube1->SetTexture("Data/textures/grasstop.png");

    Scene::Instance().AddNode(pCube1);


    // Add all the effect file names So we can loop through em
    std::string fileName = "src/particleSystem/Data/Effects/stressTest.xml";
    m_effectFileNames.push_back(fileName);

    fileName = "src/particleSystem/Data/Effects/explosion.xml";
    m_effectFileNames.push_back(fileName);

    fileName = "src/particleSystem/Data/Effects/Sparks.xml";
    m_effectFileNames.push_back(fileName);

    LoadEffect(m_effectFileNames.back());
    m_index = m_effectFileNames.size()-1;

}
void ParticleDriver::LoadWorld(){
    // Going to create a world and 
    benWorldActive = true;

    // Creating lighting effect
    LoadEffect("src/particleSystem/Data/Effects/Sparks.xml");
    bensEffects.push_back(m_pEffect);
    LoadEffect("src/particleSystem/Data/Effects/explosion.xml");
    bensEffects.push_back(m_pEffect);
    LoadEffect("src/particleSystem/Data/Effects/stressTest.xml");
    bensEffects.push_back(m_pEffect);

    // Clear existing effects (if any)
    for (Effect* effect : bensEffects) {
        Scene::Instance().RemoveNode(effect);
        delete effect;
    }
    bensEffects.clear();

    // Load and position effects in the world
    LoadEffect("src/particleSystem/Data/Effects/Sparks.xml");
    if (m_pEffect) {
        bensEffects.push_back(m_pEffect);
    }

    LoadEffect("src/particleSystem/Data/Effects/explosion.xml");
    if (m_pEffect) {
        bensEffects.push_back(m_pEffect);
    }

    LoadEffect("src/particleSystem/Data/Effects/stressTest.xml");
    if (m_pEffect) {
        bensEffects.push_back(m_pEffect);
    }

}
void ParticleDriver::LoadEffect(const std::string& filename) {
    // Clean up the existing effect
    if (m_pEffect != nullptr) {
        Scene::Instance().RemoveNode(m_pEffect);
        delete m_pEffect;
        m_pEffect = nullptr;
    }

    // Load the XML file
    TiXmlDocument doc(filename.c_str());
    if (!doc.LoadFile()) {
        std::cerr << "Failed to load effect file: " << filename << std::endl;
        return;
    }

    // Get the root element (effect node)
    const TiXmlElement* effectNode = doc.FirstChildElement("effect");
    if (!effectNode) {
        std::cerr << "Invalid effect file: " << filename << std::endl;
        return;
    }

    // Create the effect using the factory
    m_pEffect = ParticleSystemFactory::CreateEffect(effectNode);
    if (!m_pEffect) {
        std::cerr << "Failed to create effect: " << filename << std::endl;
    }
    Scene::Instance().AddNode(m_pEffect);
}
void ParticleDriver::ReloadParticleSystem() {

    if(this->isKeyJustDown('R')){
        std::cout << "Reloading Particle Files" << std::endl;

        Scene::Instance().RemoveNode(m_pEffect);
        // Reload the effect and its emitters
        if (m_pEffect) {
            delete m_pEffect;
            m_pEffect = nullptr;
        }
        
        LoadEffect(m_effectFileNames[m_index]);
    }
}
void ParticleDriver::UpdateEffect()
{
    if (this->isKeyJustDown(GLFW_KEY_SPACE))
    {
        if (!m_effectFileNames.empty())
        { 
            m_index++;
            if (m_index >= m_effectFileNames.size())
            {
                m_index = 0;
            }
            std::cout << "Reload new effect: " << m_effectFileNames[m_index] << std::endl;
            LoadEffect(m_effectFileNames[m_index]);
        }
        else
        {
            std::cerr << "[ERROR] No effect files found!" << std::endl;
        }
    }
}
void ParticleDriver::SetupCamera()
{
    m_pMainCamera = new Camera(this);
    m_pMainCamera->init();
    m_pMainCamera->setScreenSize(this->getScreenSize());
    m_pMainCamera->SetPosition(glm::vec3(0));
    m_pMainCamera->SetFOV(45);
    m_pMainCamera->SetNear(0.1f);
    m_pMainCamera->SetFar(1000.0f);
    Scene::Instance().SetActiveCamera(m_pMainCamera);
}
void ParticleDriver::SetUpTextBox()
{
    // Fps TextBox
    m_pFPSTextBox = new TextBox("src/textRendering/Data/FontData/font.fnt", "FPS", 400, 500);
    m_pFPSTextBox->setColor(glm::vec4(1, 1, 1, 1));
    m_pFPSTextBox->SetPosition(glm::vec2(850, 700));
    m_pFPSTextBox->SetHorizontalAlignment("AL_CENTER");
    m_pFPSTextBox->Init();
}
