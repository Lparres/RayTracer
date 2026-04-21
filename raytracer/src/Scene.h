#pragma once

#include "Shape.h"

class Scene : public Shape {
public:
    virtual ~Scene() = default;
    virtual bool intersect(const Ray &ray, float tMin, float tMax) const override;
    virtual bool intersect(const Ray &ray, float tMin, float tMax, HitInfo &hitInfo) const override;

    void addShape(std::shared_ptr<Shape> shape);

private:
    std::vector<std::shared_ptr<Shape>> shapes;
};
