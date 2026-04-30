#pragma once

#include "Texture.h"

// Textura hhomogénea que devuelve el mismo color para cualquier coordenada UV.
class ConstantTexture final : public Texture {
public:
    explicit ConstantTexture(Color color) : _color(color) {}

    ConstantTexture(float r, float g, float b) : _color(r, g, b) {}

    Color sample(UV) const override {
        return _color;
    }

    std::shared_ptr<Texture> clone() const override {
        return std::make_shared<ConstantTexture>(*this);
    }

private:
    Color _color;
};
