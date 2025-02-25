#pragma once

#include "../../wolf/wolf.h"
#include "../nodeClass/scene.h"
#include "pointBB.h"
#include <vector>
#include <cmath>

// A simple particle system that generates points in a circular area,
// which you can texture to simulate effects like smoke.
class CircleShapeParticle : public Node {
public:
    // Constructors
    CircleShapeParticle();
    CircleShapeParticle(glm::vec3 position);
    virtual ~CircleShapeParticle();

    // Initializes the BBPoints to use within the circle
    void Init();

    // Not used
    void Render(glm::mat4 proj, glm::mat4 view) override;
    
    // Updates the particles
    void Update(float deltaTime) override;

    // Set texture and shader not ready to be used
    // Sets a texture from file.
    void SetTexture(const std::string& texturePath);
    // Sets a shader from file.
    void SetShader(const std::string& vshPath, const std::string& fshPath);

    void SetRadius(float p_fRadius){m_radius = p_fRadius;}

    // Calculates and returns the AABB surrounding the particle system.
    AABB& CalculateVolume() override;

private:
    // Vertex structure for a single particle.

    // Generates particle positions arranged in a circle (using polar coordinates).
    std::vector<PointBBVertex> GenerateCirclePoints(int numOfPoints);

    // Axis-aligned bounding box for the particle system (for culling, etc.).
    AABB m_bounds;

    // Parameters for the particle effect.
    float m_radius = 1.0f;     // Radius of the circle in which particles are generated.


    PointBB* pPBB1 = nullptr;
    PointBB* pPBB2 = nullptr;
    PointBB* pPBB3 = nullptr;


};

