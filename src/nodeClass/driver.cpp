#include "driver.h"

Driver::Driver(const std::string& name) : wolf::App(name){
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // Prob should move the camera making out of m_App
    camera = new Camera(this);        
    camera->init();
    camera->setScreenSize(this->getScreenSize());
    camera->SetPosition(glm::vec3(0));
    camera->SetFOV(45);
    camera->SetNear(0.1f);
    camera->SetFar(1000.0f);

    enviromentList.push_back([this]() { this->GenerateObjects1(); });
    enviromentList.push_back([this]() { this->GenerateObjects(); });
    enviromentList.push_back([this]() { this->TestFrustum(); });
    enviromentList.push_back([this]() { this->Generate100Cubes(); });

    Init();

    Scene::Instance().SetActiveCamera(camera);
}
void Driver::update(float p_fDelta){
    Scene::Instance().Update(p_fDelta);

    if(this->isKeyJustDown(GLFW_KEY_R)){
        std::cout << "Reloading the scene" << std::endl;
        Scene::Instance().Clear();
        Init();
    }
    if(this->isKeyJustDown(GLFW_KEY_O)){
        std::cout << "Pressed button for OctTree" << std::endl;
        Scene::Instance().RenderOctTree();
    }
    if(this->isKeyJustDown(GLFW_KEY_P)){
        std::cout << "Pressed Buttom for Pausing frustum" << std::endl;
        Scene::Instance().GetActiveCamera()->PauseFrustum();
    }
    if(this->isKeyJustDown(GLFW_KEY_H)){
        std::cout << "Stopping rendering of cubes" << std::endl;
        Scene::Instance().RenderCubes();
    }
    if(this->isKeyJustDown(GLFW_KEY_SPACE)){
        std::cout << "Swapping scenes" << std::endl;

        Scene::Instance().Clear();
        Init();

        enviromentList[currentEnviromentIndex]();
        currentEnviromentIndex = (currentEnviromentIndex + 1) % enviromentList.size();
    }
}
void Driver::Init(){
    grid = new Grid3D(20, 1.0f);

    enviromentList[currentEnviromentIndex]();

    Scene::Instance().BuildOctTree();
}
void Driver::render(){
    // Clear the screen
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //grid->render(camera->getViewMatrix(), camera->getProjMatrix());        

    // Renders all the nodes in the scene
    Scene::Instance().Render();
}

void Driver::TestFrustum(){
    std::cout << "Loaded TestFrumtum" << std::endl;
    /*
    // This function set up a enviroment where I personally believe it's ideal to test the camera frustum
    */
   const int numCubes = 100;//was 100
   const int numChildren = 20;

   std::vector<DebugCube*> parentCubes;
   
   for (int i = 0; i < numCubes; ++i)
   {
       // Generate a random position for the parent cube
       glm::vec3 position = glm::vec3(
           rand() % 100 - 50,  // X range
           rand() % 100 - 50,  // Y range
           rand() % 100 - 50   // Z range
       );
       glm::vec3 scale = glm::vec3(1 + (rand() % 2)); // Random scale between [1, 2]
       glm::vec3 rotation = glm::vec3(0.0f); // No rotation for now (This would mess up the AABB's on the Cubes)

       DebugCube* parentCube = new DebugCube(position, scale, rotation);
       parentCube->setColor(glm::vec4(
           static_cast<float>(rand()) / RAND_MAX, // Random R
           static_cast<float>(rand()) / RAND_MAX, // Random G
           static_cast<float>(rand()) / RAND_MAX, // Random B
           1.0f  // Alpha = 1
       ));
       parentCube->setName("Parent Cube " + std::to_string(i));
       parentCube->Init();
       parentCubes.push_back(parentCube);
   }

   // Now assign children to some of the parent cubes
   for (DebugCube* parent : parentCubes)
   {
       for (int j = 0; j < numChildren; ++j)
       {
           glm::vec3 childPosition = glm::vec3(
               rand() % 50 - 25, // X offset
               rand() % 50 - 25, // Y offset
               rand() % 50 - 25  // Z offset
           );
           glm::vec3 childScale = glm::vec3(0.5f + (rand() % 3));
           glm::vec3 childRotation = glm::vec3(0.0f);

           DebugCube* childCube = new DebugCube(childPosition, childScale, childRotation);
           childCube->setColor(glm::vec4(
            (rand() % 256) / 255.0f,  
            (rand() % 256) / 255.0f, 
            (rand() % 256) / 255.0f, 
            1.0f));
                    
           childCube->setName(parent->GetName() + " Child " + std::to_string(j));
           parent->AddChild(childCube);

           childCube->Init();
       }
   }

   // Add high level nodes to the scene
   for (DebugCube* parent : parentCubes)
   {
       Scene::Instance().AddNode(parent);
   }
}
void Driver::GenerateObjects1(){
    // Create a paretn cube
    DebugCube *pCube1 = new DebugCube(glm::vec3(8), glm::vec3(0.5f), glm::vec3(0.0f));
    pCube1->setColor(glm::vec4(0, 1, 0, 1));
    pCube1->setName("Cube 1");

    // Create a second parent cube
    DebugCube *pCube2 = new DebugCube(glm::vec3(-8), glm::vec3(1.0f), glm::vec3(0.0f));
    pCube2->setColor(glm::vec4(1, 0, 0, 1));
    pCube2->setName("Cube 2");

    // Create a child cube
    DebugCube *pCube2Child = new DebugCube(glm::vec3(-3.0f), glm::vec3(0.5f), glm::vec3(0.0f));
    pCube2Child->setColor(glm::vec4(0, 0, 1, 1));
    pCube2Child->setName("Cube 2 child");
    pCube2->AddChild(pCube2Child);

    // Create a second child cube
    DebugCube *pCube3Child = new DebugCube(glm::vec3(3.0f, 2.0f, 1.0f), glm::vec3(1), glm::vec3(0.0f));
    pCube3Child->setColor(glm::vec4(0, 1, 1, 1));
    pCube3Child->setName("Cube 3 child");
    pCube2Child->AddChild(pCube3Child);
    pCube2Child->AddChild(pCube1);

    pCube1->Init();
    pCube2->Init();
    pCube2Child->Init();
    pCube3Child->Init();

    

    // We only contain high level nodes in this so we don't add the children
    Scene::Instance().AddNode(pCube1);    //It's currently a child Also the octTree breaks a bit when I uncomment this infinte recursion
    Scene::Instance().AddNode(pCube2);
}
void Driver::Generate100Cubes(){
    /*
    //  Used to test whether the frustum Is getting ride of the duplicates in the rendering process
    // By grouping all the Cubes into a single spot and Enabling the lines uncommenting the lines
    // located in scene.cpp Lines (around): 39-50
    // Which will then state how much cubes are being offically rendered and
    // How many cubes are being duplicated 
    */



    const int numCubes = 100;
    std::vector<DebugCube*> parentCubes;
    
    for (int i = 0; i < numCubes; ++i)
    {
        // Generate a random position for the parent cube
        glm::vec3 position = glm::vec3(
            rand() % 10 - 5,  // X range: [-25, 25]
            rand() % 10 - 5,  // Y range: [-25, 25]
            rand() % 10 - 5   // Z range: [-25, 25]
        );
        glm::vec3 scale = glm::vec3(1 + (rand() % 2)); // Random scale between [1, 5]
        glm::vec3 rotation = glm::vec3(0.0f); // No rotation for now

        DebugCube* parentCube = new DebugCube(position, scale, rotation);
        parentCube->setColor(glm::vec4(
            static_cast<float>(rand()) / RAND_MAX, // Random R
            static_cast<float>(rand()) / RAND_MAX, // Random G
            static_cast<float>(rand()) / RAND_MAX, // Random B
            1.0f  // Alpha = 1
        ));
        parentCube->setName("Parent Cube " + std::to_string(i));

        parentCube->Init();
        parentCubes.push_back(parentCube);
    }


    // Add only the parent nodes to the scene
    for (DebugCube* parent : parentCubes)
    {
        Scene::Instance().AddNode(parent);
    }
    
    
}
void Driver::GenerateObjects(){
    const int numCubes = 100;
    std::vector<DebugCube*> parentCubes;
    
    for (int i = 0; i < numCubes; ++i)
    {
        // Generate a random position for the parent cube
        glm::vec3 position = glm::vec3(
            rand() % 1000 - 500,  // X range: [-25, 25]
            rand() % 1000 - 500,  // Y range: [-25, 25]
            rand() % 1000 - 500   // Z range: [-25, 25]
        );
        glm::vec3 scale = glm::vec3(1 + (rand() % 5)); // Random scale between [1, 5]
        glm::vec3 rotation = glm::vec3(0.0f); // No rotation for now

        DebugCube* parentCube = new DebugCube(position, scale, rotation);
        parentCube->setColor(glm::vec4(
            static_cast<float>(rand()) / RAND_MAX, // Random R
            static_cast<float>(rand()) / RAND_MAX, // Random G
            static_cast<float>(rand()) / RAND_MAX, // Random B
            1.0f  // Alpha = 1
        ));
        parentCube->setName("Parent Cube " + std::to_string(i));

        parentCube->Init();
        parentCubes.push_back(parentCube);
    }

    // Now assign children to some of the parent cubes
    for (DebugCube* parent : parentCubes)
    {
        int numChildren = rand() % 10; // Each parent can have up to 3 children
        for (int j = 0; j < numChildren; ++j)
        {
            glm::vec3 childPosition = glm::vec3(
                rand() % 50 - 25, // X offset
                rand() % 50 - 25, // Y offset
                rand() % 50 - 25  // Z offset
            );
            glm::vec3 childScale = glm::vec3(0.5f + (rand() % 3));
            glm::vec3 childRotation = glm::vec3(0.0f);

            DebugCube* childCube = new DebugCube(childPosition, childScale, childRotation);
            childCube->setColor(glm::vec4(
                static_cast<float>(rand()) / RAND_MAX,
                static_cast<float>(rand()) / RAND_MAX,
                static_cast<float>(rand()) / RAND_MAX,
                1.0f
            ));
            childCube->setName(parent->GetName() + " Child " + std::to_string(j));
            parent->AddChild(childCube);

            childCube->Init();
        }
    }

    // Add only the parent nodes to the scene
    for (DebugCube* parent : parentCubes)
    {
        Scene::Instance().AddNode(parent);
    }
}

int RunDemoNodeClass(){
    Driver app("Node class");
    app.run();
    return 0;    
}


