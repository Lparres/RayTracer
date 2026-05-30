#include "Plane.h"
#include "glm/geometric.hpp"
#include <cmath>
#include <stdexcept>

Plane::Plane(const glm::vec3& Q, glm::vec3 side1, glm::vec3 side2, std::shared_ptr<Material> material)
    : Q(Q)
    , u(side1)
    , v(side2)
    , material(std::move(material))
{
    const glm::vec3 n = glm::cross(u, v);
    const float nLenSq = glm::dot(n, n);
    if (nLenSq <= 0.0f)
        throw std::invalid_argument("Plane: side vectors must not be parallel");

    normal = glm::normalize(n);
    D      = glm::dot(normal, Q);
    w      = n / nLenSq;
}

// --- Helpers privados ---------------------------------------------------------

float Plane::planeIntersectT(float denom, float D, const glm::vec3& normal, const Ray& ray) const
{
    if (std::fabs(denom) < 1e-8f)
        return -1.0f;
    return (D - glm::dot(normal, ray.origin())) / denom;
}

std::pair<float,float> Plane::localCoords(const glm::vec3& hitVec,
                                          const glm::vec3& u,
                                          const glm::vec3& v,
                                          const glm::vec3& w) const
{
    return { glm::dot(w, glm::cross(hitVec, v)),
             glm::dot(w, glm::cross(u, hitVec)) };
}

bool Plane::isInterior(float a, float b) const
{
    return a >= 0.f && a <= 1.f && b >= 0.f && b <= 1.f;
}

// --- Intersección  -----------------------------------------------------------

bool Plane::intersect(const Ray& ray, float tMin, float tMax) const
{
    const float denom = glm::dot(normal, ray.direction());
    const float t     = planeIntersectT(denom, D, normal, ray);
    if (t < tMin || t > tMax) return false;

    const glm::vec3 hitVec = ray.at(t) - Q;
    const auto [a, b]      = localCoords(hitVec, u, v, w);
    return isInterior(a, b);
}

bool Plane::intersect(const Ray& ray, float tMin, float tMax, HitInfo& hitInfo) const
{
    const float denom = glm::dot(normal, ray.direction());
    const float t     = planeIntersectT(denom, D, normal, ray);
    if (t < tMin || t > tMax) return false;

    const glm::vec3 intersection = ray.at(t);
    const glm::vec3 hitVec       = intersection - Q;
    const auto [a, b]            = localCoords(hitVec, u, v, w);
    if (!isInterior(a, b)) return false;

    const bool frontFace  = glm::dot(ray.direction(), normal) < 0.f;

    hitInfo.t         = t;
    hitInfo.p         = intersection;
    hitInfo.normal    = frontFace ? normal : -normal;
    hitInfo.tangent   = frontFace ? glm::normalize(u) : -glm::normalize(u);
    hitInfo.bitangent = glm::normalize(v);
    hitInfo.uv        = { a, b };
    hitInfo.material  = material;

    return true;
}
