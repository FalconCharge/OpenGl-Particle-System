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
}
ParticleDriver::~ParticleDriver(){
    delete m_pMainCamera;
}
void ParticleDriver::render(){
    // Renders all the Nodes that were added the the scene instance
    Scene::Instance().Render();
}
void ParticleDriver::Init(){
    //Transparcy
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

void ParticleDriver::BuildDemoEnviroment(){
    BuildSmoke();

    
    DebugCube *pCube1 = new DebugCube(glm::vec3(0, -5, 0), glm::vec3(5, 0.25, 5), glm::vec3(0));

    pCube1->SetName("Ground");

    pCube1->SetShader("Data/Shader/groundShader.vsh", "Data/Shader/groundShader.fsh");  //Needs to be called before init

    pCube1->Init();

    pCube1->SetTexture("Data/textures/grasstop.png");

    Scene::Instance().AddNode(pCube1);
}
void ParticleDriver::BuildSmoke(){
    CircleShapeParticle* smoke = new CircleShapeParticle(glm::vec3(0));

    smoke->Init();
    smoke->SetName("Parent Of smoke1");

    Scene::Instance().AddNode(smoke);

    CircleShapeParticle* smoke2 = new CircleShapeParticle(glm::vec3(1, 1, 0));

    smoke2->Init();
    smoke2->SetName("Parent Of smoke2");

    Scene::Instance().AddNode(smoke2);

    CircleShapeParticle* smoke3 = new CircleShapeParticle(glm::vec3(-1, -1, 0));

    smoke3->Init();
    smoke3->SetName("Parent Of smoke3");

    Scene::Instance().AddNode(smoke3);

    CircleShapeParticle* smoke4 = new CircleShapeParticle(glm::vec3(0, 1, 0));

    smoke4->Init();
    smoke4->SetName("Parent Of smoke4");

    Scene::Instance().AddNode(smoke4);

    CircleShapeParticle* smoke5 = new CircleShapeParticle(glm::vec3(1, 0, 0));

    smoke5->Init();
    smoke5->SetName("Parent Of smoke5");

    Scene::Instance().AddNode(smoke5);

    CircleShapeParticle* smoke6 = new CircleShapeParticle(glm::vec3(2, 2, 0));

    smoke6->Init();
    smoke6->SetName("Parent Of smoke6");

    Scene::Instance().AddNode(smoke6);

}
