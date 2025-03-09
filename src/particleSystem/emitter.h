#pragma once

#include <glm/glm.hpp>
#include <string>
#include "pointBB.h"    //Point Particle
#include "billBoard.h"  //Bill Board particle
#include "particle.h"
#include "affector.h"

struct Point{
    GLfloat x,y,z,w;       // Position Cords
    GLfloat r, g, b, a;
    GLfloat rotation;

    // Constructor for direct initialization
    Point(const glm::vec3& pos, const glm::vec3& color, GLfloat size, GLfloat fade, GLfloat rot)
    : x(pos.x), y(pos.y), z(pos.z), w(size),
        r(color.r), g(color.g), b(color.b), a(fade), rotation(rot) {}
};

class PointBB;
class SpawnProperties;

class Emitter : public Node{
    public:
        const int NUM_OF_PARTICLES_IN_POOL = 1000;

        Emitter();
        ~Emitter();

        // Creates a Pool of Particles which the Emitter will use
        void Init();

        // For adjusting the color, size, rotations
        void UpdateColor(glm::vec4 color){if(m_pMaterial) m_pMaterial->SetUniform("color", color);}
        void UpdateSize(float size){if(m_pMaterial) m_pMaterial->SetUniform("size", size);}
        void UpdateRotation(float rotation){if(m_pMaterial) m_pMaterial->SetUniform("Rotation", rotation);}
        void UpdateTexture(wolf::Texture* texture){if(m_pMaterial) m_pMaterial->SetTexture("texture1", texture); m_pTexture = texture;}
        void CreateTexture(std::string fileName);
        // Method Effect will call
        void Update(float p_fDelta);

        // Not implemented
        void Play();
        void Stop();
        void Pause();
        // Not implementeed

        // Returns the Material attached to the emitter
        wolf::Material* GetMaterial();


        void GetPointBB(std::vector<PointBB*>& verticesPoints);


        //Adding Affectors and removing
        void AddAffector(Affector* affector){if(affector) m_affectors.push_back(affector);}
        // Add A spawn property to be applied to the particles

        void AddSpawnProperty(SpawnProperties* spawnProperty){if(spawnProperty) m_SpawnProperties.push_back(spawnProperty);}
        // Make a remove affector soon Porbably make it by name or something

        AABB& CalculateVolume();

        void SetNumParticles(int numParticles){m_iParticles = numParticles;}
        void SetMode(std::string mode);
        void SetDuration(float duration){m_fMaxTimeAlive = duration;}
        void SetBirthRate(float birthRate){m_birthRate = birthRate;}

        void SetBurstCount(float birthCount){m_burstCount = birthCount;}
        void SetBurstInterval(float burstInterval){m_burstInterval = burstInterval;}
        void SetBurstRandom(bool random){m_burstRandom = random;}

    private:
        int m_iParticles = 1000;                    // Amount of Particles in the Pool
        std::string m_sMode = "CONTINUOUS";        // Emitter mode: "CONTINUOUS" or "BURST".
        float m_fTimeAlive = 0.0f;
        float m_fMaxTimeAlive = 20.0f;

        // For Burst Mode
        float m_burstCount = 5.0f; // Number of particles to spawn in a burst
        float m_burstInterval = 5.0f; // The amount of TimeBTW each burst
        float m_burstTime = 0.0f;   // Keeps track of how long since last burst
        bool m_burstRandom = false;



        // Variables for burst emission.
        float m_birthRate = 50.0f;
        float m_fBurstTime = 0.0f;
        float m_fBurstTimeMin = 0.0f;
        float m_fBurstTimeMax = 2.0f;
        bool m_bRandomBurstRate = false;

        // Accumulator for particle spawning.
        float m_toSpawn_accumulator = 0.0f;
        int   m_numSpawns = 0;

        // Linked lists for particle management.
        Particle* m_pFreeList = nullptr;
        Particle* m_pActiveList = nullptr;
        Particle* m_pActiveListTail = nullptr;

        // Adds a particle to the pool
        void AddToPool(Particle* p);
        // Grabs a free Particle in the Pool
        Particle* GetFreeParticle();
        // Spawns the particle into an active list to be used in rendering and updates
        void SpawnParticle();
        // Once the particle reaches the end We remove it from it's active list and add it to the pool
        void ParticleKilled(Particle* p);
        // Calculates the time the Emitter should send out a burst of particles
        void CalcBurstTime();

        // Adds the particle to the active linked list
        void AddToActive(Particle* p);
        // Adds the particle to the Pool
        void AddToFree(Particle* p);
        // Removes a Particle from Active and puts puts in the free Linked list
        void RemoveFromActive(Particle* p);
        // Removes the LRU particle
        void RemoveTail();

        // Applys the Generated Position to the Point
        virtual void SetRandomPosition(Particle* p){
            std::cout << "You didn't Add a certain type of emitter" << std::endl;
        }
        void ApplyAffectors(float p_fDelta);
        void ApplySpawnProperties(PointBB* p, const SpawnProperties* props);

        void RecycleParticle(Particle* p);
        


        //Material on the emitter
        wolf::Material* m_pMaterial = nullptr;
        std::vector<Point> m_vertexData;
        wolf::Texture* m_pTexture = nullptr;

        std::vector<Affector*> m_affectors;
        std::vector<SpawnProperties*> m_SpawnProperties;

        AABB m_bounds;


        std::vector<PointBB> m_particlePool; // Used for deleting the memory
};