#pragma once

#include <memory>

#include "Shape.h"
#include "Material.h"

// Implementación concreta de un plano que hereda de Shape
// Define la esquina (Q), dos lados (u, v) y el material del plano
// Implementa la intersección del rayo con el plano y las coordenadas locales (u,v)
class Plane : public Shape {
public:
    Plane(const glm::vec3 &Q,  glm::vec3 u,  glm::vec3 v, std::shared_ptr<Material> material);

    virtual ~Plane() = default;

    virtual bool intersect(const Ray &ray, float tMin, float tMax) const override;
    virtual bool intersect(const Ray &ray, float tMin, float tMax, HitInfo &hitInfo) const override;

    glm::vec3 getQ() const { return Q; }
    glm::vec3 getU() const { return u; }
    glm::vec3 getV() const { return v; }
    std::shared_ptr<Material> getMaterial() const { return material; }

private:
    // Función auxiliar para determinar si las coordenadas locales
    // están dentro del área del plano
    bool isInterior(float a, float b) const;

    // Función auxiliar que calcula la intersección común a ambos overloads.
    // Rellena t, alpha y beta si hay intersección válida; devuelve false en caso contrario.
    bool tryIntersect(const Ray& ray, float tMin, float tMax, float& t, float& alpha, float& beta) const;

    // Parámetros del plano
    glm::vec3 Q;
    glm::vec3 u;
    glm::vec3 v;

    // Parámetros precomputados para la intersección
    glm::vec3 w;
    glm::vec3 normal;
    float D;

    // Material del plano
    std::shared_ptr<Material> material;
};
