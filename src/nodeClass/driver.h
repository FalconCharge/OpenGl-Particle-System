#pragma once

#include <stdio.h>
#include <iostream>
#include <glm/glm.hpp>
#include "../../wolf/wolf.h"

#include "debugCube.h"
#include "camera.h"
#include "../../samplefw/Grid3D.h"
#include "lightManager.h"
#include "scene.h"
#include <unordered_map>
#include <functional>
#include <vector>

int RunDemoNodeClass();


class Driver : public wolf::App {
    public:
        Driver(const std::string& name);

        void render() override;
        void update(float f_fDelta) override;
        void Init();

        void GenerateObjects1();
        void GenerateObjects();
        void TestFrustum();
        void Generate100Cubes();


    private:
        Camera* camera;
        Grid3D* grid;
        std::vector<std::function<void()>> enviromentList;
        size_t currentEnviromentIndex = 0;  
};