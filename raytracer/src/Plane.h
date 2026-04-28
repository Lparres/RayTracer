#pragma once

#include <memory>

#include "Shape.h"
#include "Material.h"

// Implementación concreta de una esfera, que hereda de Shape
// Define el centro, radio y material de la esfera
// Implementa ecuación de la esfera para calcular intersecciones con rayos
class Plane : public Shape {
public:
    Plane(const glm::vec3 &corner,  glm::vec3 u,  glm::vec3 v, std::shared_ptr<Material> material);

    virtual ~Plane() = default;

    virtual bool intersect(const Ray &ray, float tMin, float tMax) const override;
    virtual bool intersect(const Ray &ray, float tMin, float tMax, HitInfo &hitInfo) const override;
    
    glm::vec3 get_corner() const { return corner; }
    glm::vec3 get_side1() const { return u; }
    glm::vec3 get_side2() const { return v; }
    std::shared_ptr<Material> get_material() const { return material; }
    
private:
    bool is_interior(double a, double b) const;
    bool is_interior(double a, double b, HitInfo& rec) const;

    glm::vec3 corner;
    glm::vec3 u;
    glm::vec3 v;
    glm::vec3 w;
    glm::vec3 normal;
    double D;
    std::shared_ptr<Material> material;
};
