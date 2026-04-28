#pragma once

#include <memory>

#include "Shape.h"
#include "Material.h"

// Implementación concreta de una esfera, que hereda de Shape
// Define el centro, radio y material de la esfera
// Implementa ecuación de la esfera para calcular intersecciones con rayos
class Sphere : public Shape {
public:
    Sphere(const glm::vec3 &center, float radius, std::shared_ptr<Material> material)
        : center(center), radius(radius), material(material) {}

    virtual ~Sphere() = default;

    virtual bool intersect(const Ray &ray, float tMin, float tMax) const override;
    virtual bool intersect(const Ray &ray, float tMin, float tMax, HitInfo &hitInfo) const override;

    glm::vec3 get_center() const { return center; }
    float get_radius() const { return radius; }
    std::shared_ptr<Material> get_material() const { return material; }

private:
    glm::vec3 center;
    float radius;
    std::shared_ptr<Material> material;
};
