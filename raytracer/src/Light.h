#pragma once

#include "Ray.h"
#include "Color.h"
#include "HitInfo.h"

class Light {
public:
    ~Light() = default;

    virtual Color shade(Ray r, HitInfo hit) = 0;

protected:
    Color color;

    Light(Color c) : color(c) {}
};