#include "Scene.h"

void Scene::addShape(std::shared_ptr<Shape> shape) {
    shapes.push_back(shape);
}

bool Scene::intersect(const Ray &ray, float tMin, float tMax, HitInfo &hitInfo) const {
    bool hitAnything = false;
    float closestHit = tMax;

    for (const auto &shape : shapes) {
        if (shape->intersect(ray, tMin, closestHit, hitInfo)) {
            hitAnything = true;
            closestHit = hitInfo.t;
        }
    }
    return hitAnything;
}

bool Scene::intersect(const Ray &ray, float tMin, float tMax) const {
    for (const auto &shape : shapes) {
        if (shape->intersect(ray, tMin, tMax)) {
            return true;
        }
    }
    return false;
}