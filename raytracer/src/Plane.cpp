#include "Plane.h"
#include "glm/geometric.hpp"
#include <cmath>
#include <stdexcept>

Plane::Plane(const glm::vec3 &Q,  glm::vec3 side1,  glm::vec3 side2, std::shared_ptr<Material> material) :
    Q(Q),
    u(side1),
    v(side2),
    material(material)
{
    const glm::vec3 n = glm::cross(u, v);
    const float nSquaredLength = glm::dot(n, n);
    if (nSquaredLength <= 0.0f) {
        throw std::invalid_argument("Plane side vectors cannot be parallel");
    }

    normal = glm::normalize(n);
    D = glm::dot(normal, Q);
    w = n / nSquaredLength;
}

bool Plane::intersect(const Ray &ray, float tMin, float tMax) const
{
    // Calcular el denominador para determinar si el rayo es paralelo al plano
    const float denom = glm::dot(normal, ray.direction());
    if (std::fabs(denom) < 1e-8f)
        return false;

    // Calcular el valor de t para la intersección con el plano
    const float t = (D - glm::dot(normal, ray.origin())) / denom;
    if (t < tMin || t > tMax)
        return false;

    // Determinar si el punto de intersección está dentro de los límites del plano
    // usando las coordenadas locales (alpha, beta) del plano
    const auto intersection = ray.at(t);
    const glm::vec3 planar_hitpt_vector = intersection - Q;
    const float alpha = glm::dot(w, glm::cross(planar_hitpt_vector, v));
    const float beta = glm::dot(w, glm::cross(u, planar_hitpt_vector));

    if (!isInterior(alpha, beta))
        return false;

    return true;
}

bool Plane::intersect(const Ray &ray, float tMin, float tMax, HitInfo &hitInfo) const
{
    // Calcular el denominador para determinar si el rayo es paralelo al plano
    const float denom = glm::dot(normal, ray.direction());
    if (std::fabs(denom) < 1e-8f)
        return false;

    // Calcular el valor de t para la intersección con el plano
    const float t = (D - glm::dot(normal, ray.origin())) / denom;
    if (t < tMin || t > tMax)
        return false;

    // Determinar si el punto de intersección está dentro de los límites del plano
    // usando las coordenadas locales (alpha, beta) del plano
    const auto intersection = ray.at(t);
    const glm::vec3 planar_hitpt_vector = intersection - Q;
    const float alpha = glm::dot(w, glm::cross(planar_hitpt_vector, v));
    const float beta = glm::dot(w, glm::cross(u, planar_hitpt_vector));

    if (!isInterior(alpha, beta))
        return false;

    // Rellenar hitInfo con los detalles de la intersección
    hitInfo.t = t;
    hitInfo.p = intersection;
    hitInfo.material = material;
    hitInfo.u = alpha;
    hitInfo.v = beta;
    hitInfo.normal = glm::dot(ray.direction(), normal) < 0.0f ? normal : -normal;

    return true;
}

bool Plane::isInterior(float a, float b) const {
    return a >= 0 && a <= 1 && b >= 0 && b <= 1;
}
