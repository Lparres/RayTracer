#pragma once

#include <memory>
#include "Environment.h"

class Texture;

class HDRIEnvironment : public Environment {
public:
    HDRIEnvironment(std::shared_ptr<Texture> map);
    ~HDRIEnvironment() = default;

    Color sample(const glm::vec3& rayDir) const override;

private:
    std::shared_ptr<Texture> environmentMap;
};