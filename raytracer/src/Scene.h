#pragma once

#include "Shape.h"

// Scene es un shape compuesto (patrón Composite)
// Contiene una colección de shapes y delega las intersecciones a cada una de ellas
// Se queda con el hit más cercano
class Scene : public Shape {
public:
    virtual ~Scene() = default;
    virtual bool intersect(const Ray &ray, float tMin, float tMax) const override;
    virtual bool intersect(const Ray &ray, float tMin, float tMax, HitInfo &hitInfo) const override;

    void addShape(std::shared_ptr<Shape> shape);

private:
    std::vector<std::shared_ptr<Shape>> shapes;
};
