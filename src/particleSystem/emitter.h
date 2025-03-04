#pragma once

#include <glm/glm.hpp>
#include <string>
#include "pointBB.h"    //Point Particle
#include "billBoard.h"  //Bill Board particle
#include "particle.h"
#include "affector.h"

struct Point{
    GLfloat x,y,z;       // Position Cords
};

class Emitter{
    public:
        const int NUM_OF_PARTICLES_IN_POOL = 2;

        Emitter();
        ~Emitter(){}

        // Creates a Pool of Particles which the Emitter will use
        void Init();
        // Sets the current Node with a switch statments defaulting to continuous
        void SetMode(const std::string& p_sMode);



        // Method Effect will call
        void Update(float p_fDelta);
        void Play();
        void Stop();
        void Pause();

        // Returns the Material attached to the emitter
        wolf::Material* GetMaterial();

        // Add the materials verts to verticesData
        void GetVertexData(std::vector<Point>& verticesData);


        //Adding Affectors and removing
        void AddAffector(Affector* affector){m_affectors.push_back(affector);}
        // Make a remove affector soon Porbably make it by name or something

    private:
        // Emitter mode: "CONTINUOUS" or "BURST".
        std::string m_sMode = "CONTINUOUS";

        // Variables for burst emission.
        float m_birthRate = 1.0f;
        float m_fBurstTime = 0.0f;
        float m_fBurstTimeMin = 0.0f;
        float m_fBurstTimeMax = 2.0f;
        bool m_bRandomBurstRate = false;

        // Duration the emitter has been active.
        float m_fDuration = 0.0f;

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

        void RecycleParticle(Particle* p);


        //Material on the emitter
        wolf::Material* m_pMaterial = nullptr;
        std::vector<Point> m_vertexData;
        wolf::Texture* m_pTexture = nullptr;

        std::vector<Affector*> m_affectors;
};