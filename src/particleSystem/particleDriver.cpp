#include "particleDriver.h"

#include <iostream>


int RunParticleSystemDemo(){
    ParticleDriver app("Particle System Demo");
    app.run();
    return 0;
}

ParticleDriver::ParticleDriver(const std::string& name) : wolf::App(name){
    std::cout << "Starting up particle system demo" << std::endl;
    Init();
}
void ParticleDriver::update(float p_fDelta){
    //Updates all the Nodes that were added to the scene instance
    Scene::Instance().Update(p_fDelta);

    m_pFPSTextBox->setDisplayText(std::to_string(int(this->getFPS())));

    if(this->isKeyJustDown('R')){
        std::cout << "Reloading Particle Files" << std::endl;
        ReloadParticleSystem();
    }
}
ParticleDriver::~ParticleDriver(){
    delete m_pEffect;
    for (auto& pair : m_emitters) {
        delete pair.second;
    }
    delete m_pMainCamera;
    delete m_pFPSTextBox;
}
void ParticleDriver::render(){
    // Renders all the Nodes that were added the the scene instance
    Scene::Instance().Render();

    m_pFPSTextBox->render(m_width, m_height);
}

void ParticleDriver::Init(){
    glEnable(GL_PROGRAM_POINT_SIZE);
    
    //Fps TextBox
    m_pFPSTextBox = new TextBox("src/textRendering/Data/FontData/font.fnt", "FPS", 400, 500);
    m_pFPSTextBox->setColor(glm::vec4(1, 1, 1, 1));
    m_pFPSTextBox->SetPosition(glm::vec2(850, 700));
    m_pFPSTextBox->SetHorizontalAlignment("AL_CENTER");
    m_pFPSTextBox->Init();

    // Builds the tree for the scene
    //Scene::Instance().BuildOctTree(); Ben personally doesn't want to do this right now: Feb.24

    // Sets the current cameras values for the scene
    m_pMainCamera = new Camera(this);        
    m_pMainCamera->init();
    m_pMainCamera->setScreenSize(this->getScreenSize());
    m_pMainCamera->SetPosition(glm::vec3(0));
    m_pMainCamera->SetFOV(45);
    m_pMainCamera->SetNear(0.1f);
    m_pMainCamera->SetFar(1000.0f);
    Scene::Instance().SetActiveCamera(m_pMainCamera);

    //Going to add a debug cube just to test the enviroment maybe as a floor or somthing
    BuildDemoEnviroment();


}

void ParticleDriver::CreateEffect() {
    // Load the effect from file
    LoadEffect("src/particleSystem/Data/Effects/stressTest.xml");
}



void ParticleDriver::BuildDemoEnviroment(){
    
    DebugCube *pCube1 = new DebugCube(glm::vec3(0, 0, 0), glm::vec3(10, 0.25, 10), glm::vec3(0));
    pCube1->SetName("Ground");
    pCube1->SetShader("Data/Shader/groundShader.vsh", "Data/Shader/groundShader.fsh");  //Needs to be called before init
    pCube1->Init();
    pCube1->SetTexture("Data/textures/grasstop.png");

    Scene::Instance().AddNode(pCube1);

    // Load the initial effect
    LoadEffect("src/particleSystem/Data/Effects/stressTest.xml");

}
void ParticleDriver::LoadEffect(const std::string& filename) {
    // Clean up the existing effect
    if (m_pEffect != nullptr) {
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
    Scene::Instance().RemoveNode(m_pEffect);
    // Reload the effect and its emitters
    if (m_pEffect) {
        delete m_pEffect;
        m_pEffect = nullptr;
    }
    
    CreateEffect();
}
