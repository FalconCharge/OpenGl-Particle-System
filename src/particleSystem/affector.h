#pragma once
#include "../tinyxml/tinyxml.h"
#include "particle.h"
#include "pointBB.h"


/*
//  In the furture it would be nice to add the XML creation method in here for encapsulation
*/
class Affector{
    public:
        virtual ~Affector(){}

        virtual void Apply(Particle* particle, float deltaTime) = 0;
        
    private:
};