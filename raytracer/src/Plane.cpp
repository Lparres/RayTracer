#include "Plane.h"
#include "glm/geometric.hpp"
#include <cmath>

static constexpr float PARALLEL_EPSILON = 1e-8f;

Plane::Plane(const glm::vec3 &Q,  glm::vec3 side1,  glm::vec3 side2, std::shared_ptr<Material> material) :
    Q(Q),
    u(side1),
    v(side2),
    material(material)
{
    const glm::vec3 n = glm::cross(u, v);
    normal = glm::normalize(n);
    D = glm::dot(normal, Q);
    const float n_dot_n = glm::dot(n, n);
    w = n / n_dot_n;
}

bool Plane::tryIntersect(const Ray& ray, float tMin, float tMax, float& t, float& alpha, float& beta) const
{
    // Calcular el denominador para determinar si el rayo es paralelo al plano
    const float denom = glm::dot(normal, ray.direction());
    if (std::fabs(denom) < PARALLEL_EPSILON)
        return false;

    // Calcular el valor de t para la intersección con el plano
    t = (D - glm::dot(normal, ray.origin())) / denom;
    if (t < tMin || t > tMax)
        return false;

    // Determinar si el punto de intersección está dentro de los límites del plano
    // usando las coordenadas locales (alpha, beta) del plano
    const glm::vec3 planar_hitpt_vector = ray.at(t) - Q;
    alpha = glm::dot(w, glm::cross(planar_hitpt_vector, v));
    beta  = glm::dot(w, glm::cross(u, planar_hitpt_vector));

    return isInterior(alpha, beta);
}

bool Plane::intersect(const Ray &ray, float tMin, float tMax) const
{
    float t, alpha, beta;
    return tryIntersect(ray, tMin, tMax, t, alpha, beta);
}

bool Plane::intersect(const Ray &ray, float tMin, float tMax, HitInfo &hitInfo) const
{
    float t, alpha, beta;
    if (!tryIntersect(ray, tMin, tMax, t, alpha, beta))
        return false;

    // Rellenar hitInfo con los detalles de la intersección
    hitInfo.t = t;
    hitInfo.p = ray.at(t);
    hitInfo.material = material;
    hitInfo.u = alpha;
    hitInfo.v = beta;
    hitInfo.normal = glm::dot(ray.direction(), normal) < 0.0f ? normal : -normal;

    return true;
}

bool Plane::isInterior(float a, float b) const {
    return a >= 0 && a <= 1 && b >= 0 && b <= 1;
}
