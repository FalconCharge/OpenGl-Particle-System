#include "ParticleSystemFactory.h"
#include "Effect.h" // Include the headers for your classes
#include "boxEmitter.h"
#include "PointBB.h"
#include "Affector.h"

#include "../utility/Unility.h" 

#include "scaleAffector.h"
#include "colorAffector.h"
#include "gravityAffector.h"
#include "fadeAffector.h"
#include "turbulenceAffector.h"
#include "movementAffectors.h"

// Factory method to create an Effect
Effect* ParticleSystemFactory::CreateEffect(const TiXmlElement* effectNode) {
    if(!effectNode){
        std::cout << "Effect Node is null" << std::endl;
    }
    std::string textureFile = " ";
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
    if( const char* textureAttr = effectNode->Attribute("texture"))
            textureFile = textureAttr;

    // Parse emitters
    for (const TiXmlElement* emitterNode = effectNode->FirstChildElement("emitter"); emitterNode; emitterNode = emitterNode->NextSiblingElement("emitter")) {
        // Parse the offset
        glm::vec3 offset = ParseVec3(emitterNode->Attribute("offset"));

        // Create the emitter using the factory
        Emitter* emitter = CreateEmitter(emitterNode, offset);
        if (emitter) {
            effect->AddEmitter(emitter);
            
            emitter->Init();
            emitter->CreateTexture(textureFile);
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
    if(const char* emitterSize = emitterConfigNode->Attribute("emitter_size")){
        emitter->SetBoxSize(ParseVec3(emitterSize));
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

    // Parse burst properties (if in burst mode)
    if (const char* burstCountAttr = emitterConfigNode->Attribute("burst_count")) {
        int burstCount = std::stoi(burstCountAttr); // Convert string to int
        emitter->SetBurstCount(burstCount);
    }
    if (const char* burstIntervalAttr = emitterConfigNode->Attribute("burst_interval")) {
        float burstInterval = std::stof(burstIntervalAttr); // Convert string to float
        emitter->SetBurstInterval(burstInterval);
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

    // Parse Spawn Properties
    for (const TiXmlElement* spawnPropNode = emitterConfigNode->FirstChildElement("spawn_property"); spawnPropNode; spawnPropNode = spawnPropNode->NextSiblingElement("spawn_property")) {
        
        // Create the affector using the factory
        SpawnProperties* spawnProperty = CreateSpawnProperties(spawnPropNode);
        if (spawnProperty) {
            emitter->AddSpawnProperty(spawnProperty);
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
    } else if(typeStr == "fade"){
        return CreateFadeAffector(affectorNode);
    } else if(typeStr == "turbulence"){
        return CreateTurbulenceAffector(affectorNode);
    } else if(typeStr == "directional"){
        return CreateDirectionalAffector(affectorNode);
    }
    // Add more else if statements for additional affector types.
    else {
        std::cerr << "Error: Unknown affector type: " << typeStr << std::endl;
        return nullptr;
    }
}
SpawnProperties* ParticleSystemFactory::CreateSpawnProperties(const TiXmlElement* spawnPropNode) {
    // Check if the node is valid
    if (!spawnPropNode) {
        std::cerr << "Error: SpawnProperties Node is null!" << std::endl;
        return nullptr;
    }

    // Retrieve the "name" attribute
    const char* spawnPropName = spawnPropNode->Attribute("name");
    if (!spawnPropName) {
        std::cerr << "Error: Spawn Property name is missing!" << std::endl;
        return nullptr;
    }

    // Retrieve the "type" attribute
    const char* spawnPropType = spawnPropNode->Attribute("type");
    if (!spawnPropType) {
        std::cerr << "Error: Spawn Property type is missing!" << std::endl;
        return nullptr;
    }

    // Create a new SpawnProperties object
    SpawnProperties* spawnProps = new SpawnProperties();
    spawnProps->name = spawnPropName;
    spawnProps->type = spawnPropType;

    // Parse properties based on type
    if (spawnProps->type == "random") {
        // Handle random properties
        const char* minAttr = spawnPropNode->Attribute("min");
        const char* maxAttr = spawnPropNode->Attribute("max");

        if (!minAttr || !maxAttr) {
            std::cerr << "Error: Missing 'min' or 'max' attributes for random property!" << std::endl;
            delete spawnProps;
            return nullptr;
        }

        if (spawnProps->name == "velocity" || spawnProps->name == "color") {
            // Parse vec3 values (e.g., velocity, color)
            spawnProps->minVec3 = ParseVec3(minAttr);
            spawnProps->maxVec3 = ParseVec3(maxAttr);
        } else if (spawnProps->name == "size" || spawnProps->name == "lifeTime") {
            // Parse float values (e.g., size)
            spawnProps->minFloat = ParseFloat(minAttr);
            spawnProps->maxFloat = ParseFloat(maxAttr);
        }
    } else if (spawnProps->type == "constant") {
        // Handle constant properties
        const char* valueAttr = spawnPropNode->Attribute("value");

        if (!valueAttr) {
            std::cerr << "Error: Missing 'value' attribute for constant property!" << std::endl;
            delete spawnProps;
            return nullptr;
        }

        if (spawnProps->name == "velocity" || spawnProps->name == "color") {
            // Parse vec3 values (e.g., velocity, color)
            spawnProps->constVec3 = ParseVec3(valueAttr);
        } else if (spawnProps->name == "size" || spawnProps->name == "rotation" || spawnProps->name == "lifeTime") {
            // Parse float values (e.g., size, rotation)
            spawnProps->constFloat = ParseFloat(valueAttr);
        }
    } else {
        std::cerr << "Error: Unknown property type '" << spawnProps->type << "'!" << std::endl;
        delete spawnProps;
        return nullptr;
    }

    return spawnProps;
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
    glm::vec3 startColor(1.0f), endColor(1.0f);
    
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
            startColor = ParseVec3(valueAttr);
        } else if (propName == "end") {
            endColor = ParseVec3(valueAttr);
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

Affector* ParticleSystemFactory::CreateFadeAffector(const TiXmlElement* affectorNode) {
    std::string mode;
    float startAlpha = 1.0f, endAlpha = 1.0f, midAlpha = 1.0f;
    
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
                startAlpha = std::stof(valueAttr);
            } else if (propName == "end") {
                endAlpha = std::stof(valueAttr);
            } else if(propName == "mid"){
                midAlpha = std::stof(valueAttr);
            }
        } catch (const std::exception& e) {
            std::cerr << "Error parsing ScaleAffector property " << propName << ": " << e.what() << std::endl;
            return nullptr;
        }
    }
    return new FadeAffector(startAlpha, midAlpha, endAlpha, mode);
}
Affector* ParticleSystemFactory::CreateTurbulenceAffector(const TiXmlElement* affectorNode) {
    float strength = 1.0f;
    
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
            if (propName == "strength") {
                strength = std::stof(valueAttr);
            } 
        } catch (const std::exception& e) {
            std::cerr << "Error parsing ScaleAffector property " << propName << ": " << e.what() << std::endl;
            return nullptr;
        }
    }
    return new TurbulenceAffector(strength);
}
Affector* ParticleSystemFactory::CreateDirectionalAffector(const TiXmlElement* affectorNode) {
    glm::vec3 direction(0.0f, 0.0f, 0.0f);  // Default direction
    float speed = 1.0f;

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
        } else if (propName == "speed") {
            speed = std::stof(valueAttr);  // Convert string to float for strength
        }
    }

    return new DirectionalMoveAffector(direction, speed);
}