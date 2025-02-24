#include "particleDriver.h"

#include <iostream>

int RunParticleSystemDemo(){
    ParticleDriver app("Particle System Demo");
    app.run();
    return 0;
}

ParticleDriver::ParticleDriver(const std::string& name) : wolf::App(name){
    std::cout << "Starting up particle system demo" << std::endl;
}
void ParticleDriver::update(float p_fDelta){
    return;
}
void ParticleDriver::render(){
    return;
}
void ParticleDriver::Init(){
    return;
}
