#pragma once
#include "glm/glm.hpp"
#include <sstream>

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

