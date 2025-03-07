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
}
ParticleDriver::~ParticleDriver(){
    delete m_pMainCamera;
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

void ParticleDriver::BuildDemoEnviroment(){
    //BuildSmoke(); Deprecated!! I finally can use the word!
    //CreateEffect();

    // Load the effect from an XML file (e.g., "Data/Effects/explosion.xml")
    Effect* effect = CreateEffectFromFile("src/particleSystem/Data/Effects/explosion.xml");
    if (effect) {
        Scene::Instance().AddNode(effect);
    }

    
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

    RotateAffector* rotate = new RotateAffector(2);
    emitter->AddAffector(rotate);

    m_pEffect->AddEmitter(emitter);

    m_pEffect->AddChild(emitter);

    m_pEffect->Init();  // Init all the emitters
    m_pEffect->Play();  // Start playing all emitters


    Scene::Instance().AddNode(m_pEffect);
}
// Create an Effect from an XML file with the following format:
// <effect name="explosion" position="0,0,0" shape="box" scale="1,1,1" rotation="0,0,0">
//     <emitter file="sparks.emitter" offset="0,0,1"/>
//     <emitter file="debris.emitter" offset="0,0,0"/>
//     <emitter file="fire.emitter" offset="0,0,0"/>
//     <emitter file="smoke.emitter" offset="0,4,0"/>
// </effect>
Effect* ParticleDriver::CreateEffectFromFile(const std::string& fileName){
    TiXmlDocument doc(fileName.c_str());
    if(!doc.LoadFile()){
        std::cerr <<  "Failed to load effect file: " << fileName <<std::endl;
        return nullptr;
    }


    // Gets the <effect>
    TiXmlElement* effectElem = doc.FirstChildElement("effect");
    if(!effectElem){
        std::cerr << "No effect element found in file: " << fileName << std::endl;
        return nullptr;
    }

    Effect* effect = new Effect();
    
    // Set the effect name
    if (const char* nameAttr = effectElem->Attribute("name"))
    effect->SetName(nameAttr);

    // Set the world position if provided
    if (const char* posAttr = effectElem->Attribute("position"))
    effect->SetWorldPosition(ParseVec3(std::string(posAttr)));

    // Set the world scale if provided
    if (const char* scaleAttr = effectElem->Attribute("scale"))
            effect->setWorldScale(ParseVec3(std::string(scaleAttr)));
    
    // Set the world rotation if provided
    if (const char* rotationAttr = effectElem->Attribute("rotation"))
            effect->setWorldRotation(ParseVec3(std::string(rotationAttr)));
        

    // Process each emitter in the effect
    for (TiXmlElement* emitterElem = effectElem->FirstChildElement("emitter");
        emitterElem != nullptr;
        emitterElem = emitterElem->NextSiblingElement("emitter"))
    {
        const char* emitterFile = emitterElem->Attribute("file");
        const char* offsetAttr = emitterElem->Attribute("offset");
        
        // For now, we create a default BoxEmitter.
        BoxEmitter* emitter = new BoxEmitter();
        
        // If an offset is provided, parse it.
        if (offsetAttr) {
            emitter->SetLocalPosition(ParseVec3(std::string(offsetAttr)));
        }
        
        // Add default affectors (later load these from a file too)
        ScaleAffector* scaleAffector = new ScaleAffector(500);
        emitter->AddAffector(scaleAffector);
        RotateAffector* rotateAffector = new RotateAffector(2);
        emitter->AddAffector(rotateAffector);
        GravityAffector* graivty = new GravityAffector(4.81f);
        emitter->AddAffector(graivty);
        
        // Add the emitter to the effect
        effect->AddEmitter(emitter);
        effect->AddChild(emitter);
    }
    
    // Initialize and play the effect May change these to inputs?
    effect->Init();
    effect->Play();
    
    return effect;
}

// Helper function to parse a comma-separated string into a glm::vec3
glm::vec3 ParticleDriver::ParseVec3(std::string str) {
    glm::vec3 vec(0.0f);
    std::istringstream ss(str);
    std::string token;
    int index = 0;
    while (std::getline(ss, token, ',') && index < 3) {
        vec[index++] = std::stof(token);
    }
    return vec;
}

