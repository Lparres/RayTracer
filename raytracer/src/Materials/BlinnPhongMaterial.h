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
    explicit BlinnPhongMaterial(Color albedo, float specular = 30.f, float reflectance = 0.f);

    // Construye un material con un albedo dado por una textura.
    explicit BlinnPhongMaterial(std::shared_ptr<Texture> albedoTexture, float specular = 30.f, float reflectance = 0.f);

    virtual ~BlinnPhongMaterial() = default;

    Color albedo(UV uv = {}) const override { return _albedoTexture->sample(uv); }
    float specular()          const { return _specular; }
    float reflectance()       const { return _reflectance; }
    
    // ambient + diffuse + specular
    Color evaluateDirect(const glm::vec3& wi, const glm::vec3& wo, const HitInfo&hit) const override;
    bool scatter(const Ray& incoming, const HitInfo& hit, Color& attenuation, Ray& scattered) const override;

private:
    std::shared_ptr<Texture> _albedoTexture;    // Si el color es homogéneo, usamos una ConstantTexture
    float _specular;
    float _reflectance;
};
