#pragma once
#include "Ray.h"
#include "HitInfo.h"

class Shape {
public:
    virtual ~Shape() = default;
    virtual bool intersect(const Ray &ray, float tMin, float tMax) const = 0;
    virtual bool intersect(const Ray &ray, float tMin, float tMax, HitInfo &hitInfo) const = 0;
};
