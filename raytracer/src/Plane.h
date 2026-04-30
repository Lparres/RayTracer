#pragma once

#include <memory>

#include "Shape.h"
#include "Material.h"

// Implementación concreta de un plano que hereda de Shape
// Define la esquina (Q), dos lados (u, v) y el material del plano
// Implementa la intersección del rayo con el plano y las coordenadas locales (u,v)
class Plane : public Shape {
public:
    Plane(const glm::vec3 &Q, glm::vec3 side1, glm::vec3 side2, std::shared_ptr<Material> material);

    virtual ~Plane() = default;

    virtual bool intersect(const Ray &ray, float tMin, float tMax) const override;
    virtual bool intersect(const Ray &ray, float tMin, float tMax, HitInfo &hitInfo) const override;

    glm::vec3 getQ() const { return Q; }
    glm::vec3 getU() const { return u; }
    glm::vec3 getV() const { return v; }
    std::shared_ptr<Material> getMaterial() const { return material; }

private:
    // Devuelve true si las coordenadas locales (a,b)
    // están dentro del rectángulo definido por u y v.
    bool isInterior(float a, float b) const;

    // Devuelve el parámetro t del rayo en la intersección con el plano infinito,
    // o un valor negativo si el rayo es paralelo al plano.
    float planeIntersectT(float denom,
                          float D,
                          const glm::vec3& normal,
                          const Ray& ray) const;

    // Proyecta un vector sobre el plano y devuelve sus coordenadas locales (alpha, beta).
    std::pair<float,float> localCoords(const glm::vec3& hitVec,
                                              const glm::vec3& u,
                                              const glm::vec3& v,
                                              const glm::vec3& w) const;

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
