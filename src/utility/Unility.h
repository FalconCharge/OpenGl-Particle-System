#pragma once
#include "glm/glm.hpp"
#include <sstream>
#include <random>

// Helper function to parse a comma-separated string into a glm::vec3
inline glm::vec3 ParseVec3(std::string str) {
    glm::vec3 vec(0.0f);
    std::istringstream ss(str);
    std::string token;
    int index = 0;
    while (std::getline(ss, token, ',') && index < 3) {
        vec[index++] = std::stof(token);
    }
    return vec;
}

// Helper function to parse a comma-separated string into a glm::vec4
inline glm::vec4 ParseVec4(std::string str) {
    glm::vec4 vec(0.0f);
    std::istringstream ss(str);
    std::string token;
    int index = 0;
    while (std::getline(ss, token, ',') && index < 4) {
        vec[index++] = std::stof(token);
    }
    return vec;
}

inline float ParseFloat(const std::string& str) {
    return std::stof(str);
}

// Function to generate a random float between min and max
inline float RandomFloat(float min, float max) {
    static std::random_device rd;  // Seed for the random number generator
    static std::mt19937 gen(rd()); // Mersenne Twister engine
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

// Function to generate a random glm::vec3 between min and max
inline glm::vec3 RandomVec3(const glm::vec3& min, const glm::vec3& max) {
    return glm::vec3(
        RandomFloat(min.x, max.x),
        RandomFloat(min.y, max.y),
        RandomFloat(min.z, max.z)
    );
}