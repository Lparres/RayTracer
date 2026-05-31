#pragma once

#include <memory>
#include "Environment.h"

class Texture;
struct UV;

class CubemapEnvironment : public Environment {
public:
    CubemapEnvironment(
        std::shared_ptr<Texture> top,
        std::shared_ptr<Texture> left,
        std::shared_ptr<Texture> front,
        std::shared_ptr<Texture> right,
        std::shared_ptr<Texture> back,
        std::shared_ptr<Texture> bottom
    );
    ~CubemapEnvironment() = default;

    Color sample(const glm::vec3& rayDir) const override;
    
private:
    // Normaliza [-1,1] -> [0,1]
    UV normalizeUV(float u, float v) const;

    std::shared_ptr<Texture> topTexture;
    std::shared_ptr<Texture> leftTexture;
    std::shared_ptr<Texture> frontTexture;
    std::shared_ptr<Texture> rightTexture;
    std::shared_ptr<Texture> backTexture;
    std::shared_ptr<Texture> bottomTexture;
};