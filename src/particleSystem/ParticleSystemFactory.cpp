#include "ParticleSystemFactory.h"
#include "Effect.h" // Include the headers for your classes
#include "boxEmitter.h"
#include "PointBB.h"
#include "Affector.h"

#include "../utility/Unility.h" 

#include "scaleAffector.h"
#include "colorAffector.h"
#include "gravityAffector.h"

// Factory method to create an Effect
Effect* ParticleSystemFactory::CreateEffect(const TiXmlElement* effectNode) {
    if(!effectNode){
        std::cout << "Effect Node is null" << std::endl;
    }

    Effect* effect = new Effect();

    // Set the effect name
    if (const char* nameAttr = effectNode->Attribute("name"))
            effect->SetName(nameAttr);

    // Set the world position if provided
    if (const char* posAttr = effectNode->Attribute("position"))
            effect->SetWorldPosition(ParseVec3(std::string(posAttr)));

    // Set the world scale if provided
    if (const char* scaleAttr = effectNode->Attribute("scale"))
            effect->setWorldScale(ParseVec3(std::string(scaleAttr)));
    
    // Set the world rotation if provided
    if (const char* rotationAttr = effectNode->Attribute("rotation"))
            effect->setWorldRotation(ParseVec3(std::string(rotationAttr)));
        

    // Parse emitters
    for (const TiXmlElement* emitterNode = effectNode->FirstChildElement("emitter"); emitterNode; emitterNode = emitterNode->NextSiblingElement("emitter")) {
        // Parse the offset
        glm::vec3 offset = ParseVec3(emitterNode->Attribute("offset"));

        // Create the emitter using the factory
        Emitter* emitter = CreateEmitter(emitterNode, offset);
        if (emitter) {
            effect->AddEmitter(emitter);
            emitter->Init();
        }
    }
    return effect;
}

// Factory method to create an Emitter
Emitter* ParticleSystemFactory::CreateEmitter(const TiXmlElement* emitterNode, const glm::vec3& offset) {
    if(!emitterNode){
        std::cerr <<"Emitter Node is null" << std::endl;
        return nullptr;
    }

    // Parse the emitter file path
    const char* emitterFile = emitterNode->Attribute("file");
    if (!emitterFile) {
        std::cerr << "Emitter file path is missing" << std::endl;
        return nullptr;
    }

    // Load the emitter XML file
    TiXmlDocument doc(emitterFile);
    if (!doc.LoadFile()) {
        std::cerr << "Failed to load emitter file: " << emitterFile << std::endl;
        return nullptr;
    }

    // Parse the emitter node from the loaded file
    const TiXmlElement* emitterConfigNode = doc.FirstChildElement("emitter");
    if (!emitterConfigNode) {
        std::cerr << "Invalid emitter file: " << emitterFile << std::endl;
        return nullptr;
    }

    // Create the Emitter
    BoxEmitter* emitter = new BoxEmitter();

    // Parse and set emitter properties
    if (const char* nameAttr = emitterConfigNode->Attribute("name")) {
        emitter->SetName(nameAttr);
    }
    if (const char* numParticlesAttr = emitterConfigNode->Attribute("num_particles")) {
        int numParticles = std::stoi(numParticlesAttr); // Convert string to int
        emitter->SetNumParticles(numParticles);
    }
    if (const char* durationAttr = emitterConfigNode->Attribute("duration")) {
        float duration = std::stof(durationAttr); // Convert string to float
        emitter->SetDuration(duration);
    }
    if (const char* typeAttr = emitterConfigNode->Attribute("type")) {
        emitter->SetMode(typeAttr);
    }
    if (const char* birthRateAttr = emitterConfigNode->Attribute("birthrate")) {
        float birthRate = std::stof(birthRateAttr); // Convert string to float
        emitter->SetBirthRate(birthRate);
    }

    emitter->SetLocalPosition(offset);

    // Parse affectors
    for (const TiXmlElement* affectorNode = emitterConfigNode->FirstChildElement("affector"); affectorNode; affectorNode = affectorNode->NextSiblingElement("affector")) {
        
        // Create the affector using the factory
        Affector* affector = CreateAffector(affectorNode);
        if (affector) {
            emitter->AddAffector(affector);
        }
    }


    return emitter; // Allocate memory and return raw pointer
}

// Factory method to create a PointBB Not implemented
PointBB* ParticleSystemFactory::CreatePointBB(const TiXmlElement* pointBBNode) {
    return new PointBB(); // Allocate memory and return raw pointer
}

// Factory method to create an Affector
Affector* ParticleSystemFactory::CreateAffector(const TiXmlElement* affectorNode) {
    if (!affectorNode) {
        std::cerr << "Error: Affector Node is null!" << std::endl;
        return nullptr;
    }

    // Retrieve the "type" attribute
    const char* affectorType = affectorNode->Attribute("type");
    if (!affectorType) {
        std::cerr << "Error: Affector type is missing!" << std::endl;
        return nullptr;
    }

    std::string typeStr(affectorType);

    if (typeStr == "scale") {
        return CreateScaleAffector(affectorNode);
    } else if (typeStr == "color") {
        return CreateColorAffector(affectorNode);
    } else if(typeStr == "gravity"){
        return CreateGravityAffector(affectorNode);
    }
    // Add more else if statements for additional affector types.
    else {
        std::cerr << "Error: Unknown affector type: " << typeStr << std::endl;
        return nullptr;
    }
}

Affector* ParticleSystemFactory::CreateScaleAffector(const TiXmlElement* affectorNode) {
    std::string mode;
    float startScale = 1.0f, endScale = 1.0f;
    
    // Loop through each property element to extract values.
    for (const TiXmlElement* property = affectorNode->FirstChildElement("property"); 
         property; property = property->NextSiblingElement("property")) {
        
        const char* nameAttr = property->Attribute("name");
        const char* valueAttr = property->Attribute("value");
        if (!nameAttr || !valueAttr) {
            std::cerr << "Warning: Missing property name or value in ScaleAffector" << std::endl;
            continue;
        }
        std::string propName(nameAttr);
        try {
            if (propName == "mode") {
                mode = valueAttr;
            } else if (propName == "start") {
                startScale = std::stof(valueAttr);
            } else if (propName == "end") {
                endScale = std::stof(valueAttr);
            }
        } catch (const std::exception& e) {
            std::cerr << "Error parsing ScaleAffector property " << propName << ": " << e.what() << std::endl;
            return nullptr;
        }
    }
    return new ScaleAffector(startScale, endScale, mode);
}
Affector* ParticleSystemFactory::CreateColorAffector(const TiXmlElement* affectorNode) {
    std::string mode;
    glm::vec4 startColor(1.0f), endColor(1.0f);
    
    for (const TiXmlElement* property = affectorNode->FirstChildElement("property"); 
         property; property = property->NextSiblingElement("property")) {
        
        const char* nameAttr = property->Attribute("name");
        const char* valueAttr = property->Attribute("value");
        if (!nameAttr || !valueAttr) {
            std::cerr << "Warning: Missing property name or value in ColorAffector" << std::endl;
            continue;
        }
        std::string propName(nameAttr);
        if (propName == "mode") {
            mode = valueAttr;
        } else if (propName == "start") {
            startColor = ParseVec4(valueAttr);
        } else if (propName == "end") {
            endColor = ParseVec4(valueAttr);
        }
    }
    return new ColorAffector(startColor, endColor, mode);
}
Affector* ParticleSystemFactory::CreateGravityAffector(const TiXmlElement* affectorNode) {
    glm::vec3 direction(0.0f, -9.81f, 0.0f);  // Default direction
    float strength = 9.81f;  // Default strength

    for (const TiXmlElement* property = affectorNode->FirstChildElement("property"); 
         property; property = property->NextSiblingElement("property")) {
        
        const char* nameAttr = property->Attribute("name");
        const char* valueAttr = property->Attribute("value");
        if (!nameAttr || !valueAttr) {
            std::cerr << "Warning: Missing property name or value in GravityAffector" << std::endl;
            continue;
        }
        std::string propName(nameAttr);

        if (propName == "direction") {
            direction = ParseVec3(valueAttr);  // Helper function to parse the direction vector
        } else if (propName == "strength") {
            strength = std::stof(valueAttr);  // Convert string to float for strength
        }
    }

    return new GravityAffector(strength);
}

