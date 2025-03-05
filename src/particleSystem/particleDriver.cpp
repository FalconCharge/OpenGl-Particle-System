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
    glEnable(GL_PROGRAM_POINT_SIZE);
    
    //Transparcy
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
    //BuildSmoke(); Deprecated!! I finally can use the word!
    CreateEffect();

    
    DebugCube *pCube1 = new DebugCube(glm::vec3(0, -5, 0), glm::vec3(5, 0.25, 5), glm::vec3(0));

    pCube1->SetName("Ground");

    pCube1->SetShader("Data/Shader/groundShader.vsh", "Data/Shader/groundShader.fsh");  //Needs to be called before init

    pCube1->Init();

    pCube1->SetTexture("Data/textures/grasstop.png");

    Scene::Instance().AddNode(pCube1);
}
void ParticleDriver::BuildSmoke(){
    CircleShapeParticle* smoke = new CircleShapeParticle(glm::vec3(20));

    smoke->Init();
    smoke->SetName("Parent Of smoke1");

    Scene::Instance().AddNode(smoke);

}
void ParticleDriver::CreateEffect(){
    Effect* m_pEffect = new Effect();
    m_pEffect->setWorldRotation(glm::vec3(45));
    m_pEffect->setWorldScale(glm::vec3(5));

    //Create the emitter
    BoxEmitter* emitter = new BoxEmitter();

    MoveForwardAffector* forward = new MoveForwardAffector(4);
    //emitter->AddAffector(forward);

    GravityAffector* graivty = new GravityAffector(4.81f);
    //emitter->AddAffector(graivty);

    ScaleAffector* scale = new ScaleAffector(500);
    emitter->AddAffector(scale);

    RotateAffector* rotate = new RotateAffector(50);
    emitter->AddAffector(rotate);

    m_pEffect->AddEmitter(emitter);

    m_pEffect->AddChild(emitter);

    m_pEffect->Init();  // Init all the emitters
    m_pEffect->Play();  // Start playing all emitters


    Scene::Instance().AddNode(m_pEffect);
}

