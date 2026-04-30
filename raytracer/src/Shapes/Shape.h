#pragma once
#include "Ray.h"
#include "HitInfo.h"

// Interfaz abstracta para cualquier forma geométrica que pueda ser intersectada por un rayo
// Define dos métodos de intersección:
//      uno que solo verifica si hay intersección (para sombras, más eficiente)
//      otro que devuelve un HitInfo completo (para iluminación, más costoso)
class Shape {
public:
    virtual ~Shape() = default;
    virtual bool intersect(const Ray &ray, float tMin, float tMax) const = 0;
    virtual bool intersect(const Ray &ray, float tMin, float tMax, HitInfo &hitInfo) const = 0;
};
