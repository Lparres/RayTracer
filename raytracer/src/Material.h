#pragma once

#include "Color.h"

class Material {
public:
    Material(const Color& albedo) : albedo(albedo) {}
    virtual ~Material() = default;

    Color get_albedo() const { return albedo; }

private:
    Color albedo;
};