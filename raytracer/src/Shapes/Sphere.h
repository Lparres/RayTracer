#pragma once

#include <memory>
#include "Shape.h"
#include "Material.h"

// Esfera definida por su centro, radio y material.
class Sphere final : public Shape {
public:
    Sphere(glm::vec3 center, float radius, std::shared_ptr<Material> material);

    bool intersect(const Ray& ray, float tMin, float tMax) const override;
    bool intersect(const Ray& ray, float tMin, float tMax, HitInfo& hitInfo) const override;

    glm::vec3                  center()   const { return _center; }
    float                      radius()   const { return _radius; }
    std::shared_ptr<Material>  material() const { return _material; }

private:
    // Resuelve la ecuación cuadrática de intersección rayo-esfera
    // y devuelve la raíz más cercana en (tMin, tMax),
    // o un valor negativo si no existe una intersección válida.
    float nearestRoot(const Ray& ray, float tMin, float tMax) const;

    // Computes UV coordinates from a unit outward normal using spherical projection.
    // Obtiene cordenadas UV a partir de una normal unitaria
    // usando la proyección esférica.
    UV sphericalUV(const glm::vec3& normal) const;

    glm::vec3                 _center;
    float                     _radius;
    std::shared_ptr<Material> _material;
};
