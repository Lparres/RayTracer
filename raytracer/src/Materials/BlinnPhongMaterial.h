#pragma once

#include <memory>
#include "Material.h"
#include "Color.h"
#include "Texture.h"
#include "ConstantTexture.h"

// Legacy. Modelo de iluminación de Blinn-Phong
class BlinnPhongMaterial : public Material {
public:
    // Construye un material con un albedo homogéneo.
    explicit BlinnPhongMaterial(Color albedo, float shininess = 30.f, float reflectance = 0.f, float ks = 0.5f);

    // Construye un material con un albedo dado por una textura.
    explicit BlinnPhongMaterial(std::shared_ptr<Texture> albedoTexture, float shininess = 30.f, float reflectance = 0.f, float ks = 0.5f);

    virtual ~BlinnPhongMaterial() = default;

    Color albedo(UV uv = {}) const override { return _albedoTexture->sample(uv); }
    float shininess()         const { return _shininess; }
    float reflectance()       const { return _reflectance; }
    float ks()                const { return _ks; }

    // ambient + diffuse + specular
    Color evaluateDirect(const glm::vec3& wi, const glm::vec3& wo, const HitInfo&hit) const override;
    bool scatter(const Ray& incoming, const HitInfo& hit, Color& attenuation, Ray& scattered) const override;

private:
    std::shared_ptr<Texture> _albedoTexture;    // Si el color es homogéneo, usamos una ConstantTexture
    float _shininess;
    float _reflectance;
    float _ks;
};
