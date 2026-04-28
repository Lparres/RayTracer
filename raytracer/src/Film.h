#pragma once

#include "Color.h"
#include <vector>
#include <stdexcept>

// Film es el lienzo de la imagen final.
// Define el tamaño de la imagen y almacena el color de cada píxel.
class Film {
public:
    Film(int width, int height)
        : _width(width)
        , _height(height)
        , _pixels(width * height, Color(0, 0, 0))
    {}

    void setPixel(int x, int y, Color color) {
        _pixels[y * _width + x] = color;
    }

    Color getPixel(int x, int y) const {
        return _pixels[y * _width + x];
    }

    int getWidth()  const { return _width; }
    int getHeight() const { return _height; }
    float getAspectRatio() const { return static_cast<float>(_width) / _height; }
    
private:
    int _width;
    int _height;
    std::vector<Color> _pixels;
};