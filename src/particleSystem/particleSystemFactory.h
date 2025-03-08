#pragma once
#include <string>
#include "../tinyxml/tinyxml.h"
#include <glm/glm.hpp>


// Forward declarations of your particle system classes
class Effect;
class Emitter;
class PointBB;
class Affector;

class ParticleSystemFactory {
public:
    // Factory method to create an Effect
    static Effect* CreateEffect(const TiXmlElement* effectNode);

    // Factory method to create an Emitter
    static Emitter* CreateEmitter(const TiXmlElement* emitterNode, const glm::vec3& offset);

    // Factory method to create a PointBB
    static PointBB* CreatePointBB(const TiXmlElement* pointNode);

    // Factory method to create an Affector
    static Affector* CreateAffector(const TiXmlElement* affectorNode);

private:

    static Affector* CreateScaleAffector(const TiXmlElement* affectorNode);
    static Affector* CreateColorAffector(const TiXmlElement* affectorNode);
    static Affector* CreateGravityAffector(const TiXmlElement* affectorNode);
    static Affector* CreateFadeAffector(const TiXmlElement* affectorNode);

    


    // Continue to add me here
};