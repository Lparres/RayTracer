#pragma once

#include "Texture.h"

// Textura hhomogénea que devuelve el mismo color para cualquier coordenada UV.
class ConstantTexture final : public Texture {
// Factoría
public:
    static std::shared_ptr<ConstantTexture> createTexture(Color color) {
        return std::make_shared<ConstantTexture>(Private(), color);
    }

    static std::shared_ptr<ConstantTexture> createTexture(float r, float g, float b) {
        return std::make_shared<ConstantTexture>(Private(), r,g,b);
    }

public:
    explicit ConstantTexture(Private p, Color color) : _color(color) {}

    ConstantTexture(Private p, float r, float g, float b) : _color(r, g, b) {}

    Color sample(UV) const override {
        return _color;
    }

private:
    Color _color;
};
