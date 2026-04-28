#include "Plane.h"
#include "glm/geometric.hpp"
#include <iostream>

Plane::Plane(const glm::vec3 &corner,  glm::vec3 side1,  glm::vec3 side2, std::shared_ptr<Material> material)
    : corner(corner), u(side1), v(side2), material(material) {
        normal = glm::normalize(glm::cross(u, v));
        D = -glm::dot(normal, corner);
        auto n = glm::cross(glm::normalize(u), glm::normalize(v));
        w = n/glm::dot(n, n);
    }

bool Plane::intersect(const Ray &ray, float tMin, float tMax) const {
    // Implementación de la intersección del rayo con el plano
    auto denom = glm::dot(normal, ray.direction());
    if (std::fabs(denom) < 1e-8) {
        return false; // El rayo es paralelo al plano
    }

    auto t = (D - glm::dot(normal, ray.origin())) / denom;
    if (t < tMin || t > tMax)
        return false;

    // Determine if the hit point lies within the planar shape using its plane coordinates.
    auto intersection = ray.at(t);
    glm::vec3 planar_hitpt_vector = intersection - corner;
    auto alpha = glm::dot(w, glm::cross(planar_hitpt_vector, v));
    auto beta = glm::dot(w, glm::cross(u, planar_hitpt_vector));

    if (!is_interior(alpha, beta))
        return false;

    return true;
}

bool Plane::intersect(const Ray &ray, float tMin, float tMax, HitInfo &hitInfo) const {
    // Implementación de la intersección del rayo con el plano
    auto denom = glm::dot(normal, ray.direction());
    if (std::fabs(denom) < 1e-8) {
        std::cout << "Ray is parallel to plane, denom=" << denom << std::endl;
        return false; // El rayo es paralelo al plano
    }

    auto t = (D - glm::dot(normal, ray.origin())) / denom;
    if (t < tMin || t > tMax) {
        // std::cout << "Ray intersects plane at t=" << t << " but outside tMin=" << tMin << " and tMax=" << tMax << std::endl;
        return false;
    }

    // Determine if the hit point lies within the planar shape using its plane coordinates.
    auto intersection = ray.at(t);
    glm::vec3 planar_hitpt_vector = intersection - corner;
    auto alpha = glm::dot(w, glm::cross(planar_hitpt_vector, v));
    auto beta = glm::dot(w, glm::cross(u, planar_hitpt_vector));

    if (!is_interior(alpha, beta, hitInfo)) {
        return false;
        std::cout << "Hit plane at t=" << t << " but outside shape with alpha=" << alpha << " and beta=" << beta << std::endl;
    }

    // Ray hits the 2D shape; set the rest of the hit record and return true.
    hitInfo.t = t;
    hitInfo.p = intersection;
    hitInfo.material = material;
    hitInfo.normal = normal;
    std::cout << "Hit plane at t=" << t << " with alpha=" << alpha << " and beta=" << beta << std::endl;
    return true;
}

bool Plane::is_interior(double a, double b) const {
    if(a < 0 || a > 1 || b < 0 || b > 1)
        return false;

    return true;
}


bool Plane::is_interior(double a, double b, HitInfo& hitInfo) const {
    // Given the hit point in plane coordinates, return false if it is outside the
    // primitive, otherwise set the hit record UV coordinates and return true.
    if(a < 0 || a > 1 || b < 0 || b > 1)
        return false;

    hitInfo.u = a;
    hitInfo.v = b;
    return true;
}
