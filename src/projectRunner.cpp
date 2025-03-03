#include "ProjectRunner.h"

#include <filesystem>
//Helper funciton to start the code from main.cpp
int RunMainProject(){
    ProjectRunner app("Particle System");
    app.run();
    return 0;
}

ProjectRunner::ProjectRunner(const std::string& name) : wolf::App(name){
    std::cout << "Getting Main Project Ready" << std::endl;
    //Getting the project ready to run
    Init();
}
ProjectRunner::~ProjectRunner(){
    // Clean up the camera
    delete m_pMainCamera;
}

void ProjectRunner::Init(){
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
    BuildBasicEnviroment();

}
void ProjectRunner::update(float p_fDelta){
    //Updates all the Nodes that were added to the scene instance
    Scene::Instance().Update(p_fDelta);
}
void ProjectRunner::render(){
    // Renders all the Nodes that were added the the scene instance
    Scene::Instance().Render();
}
void ProjectRunner::BuildBasicEnviroment(){
    DebugCube *pCube1 = new DebugCube(glm::vec3(-1), glm::vec3(5, 0.25, 5), glm::vec3(50));

    pCube1->SetName("Ground");

    pCube1->SetShader("Data/Shader/groundShader.vsh", "Data/Shader/groundShader.fsh");  //Needs to be called before init

    pCube1->Init();

    pCube1->SetTexture("Data/textures/grasstop.png");

    Scene::Instance().AddNode(pCube1);

    // Bulding a BB
    BillBoard *pBB1 = new BillBoard(glm::vec3(5, 5, 5), glm::vec3(1), glm::vec3(0.0));

    pBB1->SetName("BillBoard");

    //Don't need to bother with the shader

    pBB1->SetColor(glm::vec4(0, 0, 0, 1)); // White
    pBB1->SetTexture("Data/textures/particles/smoke_01.png");

    pBB1->SetShader("Data/Shader/DebugCube.vsh", "Data/Shader/DebugCube.fsh");

    pBB1->Init();

    Scene::Instance().AddNode(pBB1);
    // NO longer works since I stop deriveing from node
    /*
    //Building Point pixels
    PointBB *pPBB1 = new PointBB(glm::vec3(2), glm::vec3(1), glm::vec3(0.0));

    pPBB1->SetName("BB with points");

    //Don't need to bother with the shader

    pPBB1->SetColor(glm::vec4(0, 0, 0, 1)); // White
    pPBB1->SetTexture("Data/textures/particles/smoke_01.png");

    pPBB1->SetShader("Data/Shader/glPoint.vsh", "Data/Shader/glPoint.fsh");

    pPBB1->Init();

    Scene::Instance().AddNode(pPBB1);
    */
}

