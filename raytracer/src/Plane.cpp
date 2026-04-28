#include "Plane.h"
#include "glm/geometric.hpp"
#include <iostream>

Plane::Plane(const glm::vec3 &corner,  glm::vec3 side1,  glm::vec3 side2, std::shared_ptr<Material> material)
    : corner(corner), u(side1), v(side2), material(material) {
        normal = glm::normalize(glm::cross(u, v));
        D = glm::dot(normal, corner);
        const glm::vec3 n = glm::cross(u, v);
        w = n / glm::dot(n, n);
    }

bool Plane::intersect(const Ray &ray, float tMin, float tMax) const {
    // Implementación de la intersección del rayo con el plano
    const float denom = glm::dot(normal, ray.direction());
    if (std::fabs(denom) < 1e-8) {
        return false; // El rayo es paralelo al plano
    }

    const float t = (D - glm::dot(normal, ray.origin())) / denom;
    if (t < tMin || t > tMax)
        return false;

    // Determine if the hit point lies within the planar shape using its plane coordinates.
    const auto intersection = ray.at(t);
    const glm::vec3 planar_hitpt_vector = intersection - corner;
    const float alpha = glm::dot(w, glm::cross(planar_hitpt_vector, v));
    const float beta = glm::dot(w, glm::cross(u, planar_hitpt_vector));

    if (!isInterior(alpha, beta))
        return false;

    return true;
}

bool Plane::intersect(const Ray &ray, float tMin, float tMax, HitInfo &hitInfo) const {
    // Implementación de la intersección del rayo con el plano
    const float denom = glm::dot(normal, ray.direction());
    if (std::fabs(denom) < 1e-8) {
        return false; // El rayo es paralelo al plano
    }

    const float t = (D - glm::dot(normal, ray.origin())) / denom;
    if (t < tMin || t > tMax) {
        return false;
    }

    // Determine if the hit point lies within the planar shape using its plane coordinates.
    const auto intersection = ray.at(t);
    const glm::vec3 planar_hitpt_vector = intersection - corner;
    const float alpha = glm::dot(w, glm::cross(planar_hitpt_vector, v));
    const float beta = glm::dot(w, glm::cross(u, planar_hitpt_vector));

    if (!isInterior(alpha, beta)) {
        return false;
    }

    // Ray hits the 2D shape; set the rest of the hit record and return true.
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
