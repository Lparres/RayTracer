#pragma once
#include "Color.h"

class Environment {
public:
    virtual ~Environment() = default;
    virtual Color sample(const glm::vec3& rayDir) const = 0;
};