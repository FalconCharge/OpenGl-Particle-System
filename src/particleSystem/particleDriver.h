#pragma once

#include "../../wolf/wolf.h"
#include <string>

//Helper for the runner
int RunParticleSystemDemo();

class ParticleDriver : public wolf::App {
    public:
        ParticleDriver(const std::string& name);

        //I'd like to go into wolf sometime to change the update --> Update and render --> Render;
        void render() override;
        void update(float p_fDelta) override;
        void Init();

    private:

};