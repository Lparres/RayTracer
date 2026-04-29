#include "Sphere.h"
#include "glm/geometric.hpp"
#include <cmath>

namespace {
    constexpr float PI = 3.14159265358979323846f;
}

Sphere::Sphere(glm::vec3 center, float radius, std::shared_ptr<Material> material)
    : _center(center)
    , _radius(radius)
    , _material(std::move(material))
{}

// --- Helpers privados ---------------------------------------------------------

float Sphere::nearestRoot(const Ray& ray, float tMin, float tMax) const
{
    // Forma optimizada de la cuadrática rayo-esfera: sustituyendo h = dot(d, oc)
    // se reduce a la mitad b y se cancela el factor de 4 en el discriminante.
    const glm::vec3 oc = _center - ray.origin();
    const float a      = glm::dot(ray.direction(), ray.direction());
    const float h      = glm::dot(ray.direction(), oc);
    const float c      = glm::dot(oc, oc) - _radius * _radius;

    const float discriminant = h * h - a * c;
    if (discriminant < 0.0f) return -1.0f;

    const float sqrtd = std::sqrt(discriminant);

    // Revisamos ambas raíces, empezando por la más cercana (h - sqrtd) / a.
    for (const float root : { (h - sqrtd) / a, (h + sqrtd) / a }) {
        if (root > tMin && root < tMax) return root;
    }

    return -1.0f;
}

UV Sphere::sphericalUV(const glm::vec3& n) const
{
    // Proyección esférica estándar:
    //   u: longitud en [0,1], este desde el eje -X
    //   v: latitud  en [0,1], norte desde el polo sur
    return {
        0.5f + std::atan2(n.z, n.x) / (2.0f * PI),
        0.5f - std::asin(n.y) / PI
    };
}

// --- Intersección -----------------------------------------------------------

bool Sphere::intersect(const Ray& ray, float tMin, float tMax) const
{
    return nearestRoot(ray, tMin, tMax) >= 0.0f;
}

bool Sphere::intersect(const Ray& ray, float tMin, float tMax, HitInfo& hitInfo) const
{
    const float t = nearestRoot(ray, tMin, tMax);
    if (t < 0.0f) return false;

    hitInfo.t        = t;
    hitInfo.p        = ray.at(t);
    hitInfo.material = _material;

    const glm::vec3 outwardNormal = glm::normalize(hitInfo.p - _center);
    hitInfo.normal = glm::dot(ray.direction(), outwardNormal) < 0.0f
                         ? outwardNormal
                         : -outwardNormal;
    hitInfo.uv = sphericalUV(outwardNormal);

    return true;
}
