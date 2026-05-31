#pragma once

#include "Environment.h"
#include "glm/geometric.hpp"

class GradientEnvironment : public Environment {
public:
    GradientEnvironment() {}
    ~GradientEnvironment() = default;

    Color sample(const glm::vec3& rayDir) const override {
            
        glm::vec3 unitDirection = glm::normalize(rayDir);
        float a = 0.5f * (unitDirection.y + 1.0f);
        return (1.0f - a) * Color(1.0f, 1.0f, 1.0f) + a * Color(0.5f, 0.7f, 1.0f);
        
    }
};