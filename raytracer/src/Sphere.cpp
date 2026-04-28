#include "Sphere.h"
#include "glm/geometric.hpp"

bool Sphere::intersect(const Ray &ray, float tMin, float tMax) const {
    glm::vec3 oc = center - ray.origin();
    auto a = glm::dot(ray.direction(), ray.direction());
    auto b = -2.0 * glm::dot(ray.direction(), oc);
    auto c = glm::dot(oc, oc) - radius*radius;
    auto discriminant = b*b - 4*a*c;
    if (discriminant < 0) {
        return false;
    } else {
        float root = (-b - sqrt(discriminant)) / (2.0*a);
        if (root < tMax && root > tMin)
            return true;
        root = (-b + sqrt(discriminant)) / (2.0*a);
        if (root < tMax && root > tMin) {
            return true;
        }
        
    }
    return false;
}

bool Sphere::intersect(const Ray &ray, float tMin, float tMax, HitInfo &hitInfo) const {

    //version simplificada

    glm::vec3 oc = center - ray.origin();
    float a = glm::dot(ray.direction(), ray.direction());
    float h = glm::dot(ray.direction(), oc);
    float c = glm::dot(oc, oc) - radius * radius;

    float discriminant = h*h - a*c;
    if (discriminant < 0.0f)
        return false;

    float sqrtd = sqrt(discriminant);

    // Find nearest root in range
    float root = (h - sqrtd) / a;
    if (root <= tMin || root >= tMax) {
        root = (h + sqrtd) / a;
        if (root <= tMin || root >= tMax)
            return false;
    }

    hitInfo.t = root;
    hitInfo.p = ray.at(hitInfo.t);

    // Normal
    hitInfo.normal = glm::normalize((hitInfo.p - center) / radius);

    hitInfo.material = material;

    return true;
}
