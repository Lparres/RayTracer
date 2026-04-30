#pragma once

#include <memory>
#include "Color.h"
#include "Texture.h"
#include "ConstantTexture.h"

// Material representa las propiedades ópticas de una superficie.
class Material {
public:
    // Construye un material con un albedo homogéneo.
    explicit Material(Color albedo, float specular = 30.f, float reflectance = 0.f)
        : _albedoTexture(std::make_shared<ConstantTexture>(albedo))
        , _specular(specular)
        , _reflectance(reflectance)
    {}

    // Construye un material con un albedo dado por una textura.
    explicit Material(const Texture& albedoTexture, float specular = 30.f, float reflectance = 0.f)
        : _albedoTexture(albedoTexture.clone())
        , _specular(specular)
        , _reflectance(reflectance)
    {}

    virtual ~Material() = default;

    Color albedo(UV uv = {}) const { return _albedoTexture->sample(uv); }
    float specular()          const { return _specular; }
    float reflectance()       const { return _reflectance; }

private:
    std::shared_ptr<Texture> _albedoTexture;    // Si el color es homogéneo, usamos una ConstantTexture
    float _specular;
    float _reflectance;
};
