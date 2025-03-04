#pragma once
#include "emitter.h"



class Affector{
    public:
        virtual ~Affector(){}

        virtual void Apply(Particle* particle, float deltaTime) = 0;

        private:
};