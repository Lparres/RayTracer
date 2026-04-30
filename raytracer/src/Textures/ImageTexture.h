#pragma once

#include <string>  
#include "Texture.h"
#include "Image.h"

// Textura que obtiene el color de una imagen cargada
class ImageTexture final : public Texture {
public:
    explicit ImageTexture(const std::string& filename) : image(filename) {}

    Color sample(UV uv) const override {
        // Si la imagen no se ha cargado correctamente, devuelve magenta
        if(image.width() == 0) return Color(1.0f, 0.0f, 1.0f); 

        const float cu = clamp(uv.u);
        const float cv = 1.0f - clamp(uv.v); // invertir coord v

        int x = cu * image.width();
        int y = cv * image.height();
        auto data = image.pixel_data(x, y);

        const float colorScale = 1.0f / 255.0f;
        return Color(data[0]*colorScale, data[1]*colorScale, data[2]*colorScale); 
    }

    std::shared_ptr<Texture> clone() const override {
        return std::make_shared<ImageTexture>(*this);
    }

private:
    Image image;
};
