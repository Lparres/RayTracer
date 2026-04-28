#pragma once

#include "Color.h"
#include <cassert>
#include <vector>

// Film es el lienzo de la imagen final.
// Define el tamaño de la imagen y almacena el color de cada píxel.
class Film {
public:
    Film(int width, int height)
        : _width(width)
        , _height(height)
        , _pixels(width * height, Color(0, 0, 0))
    {
        assert(width > 0);
        assert(height > 0);
    }

    void setPixel(int x, int y, Color color) {
        assert(x >= 0 && x < _width);
        assert(y >= 0 && y < _height);
        _pixels[y * _width + x] = color;
    }

    Color getPixel(int x, int y) const {
        assert(x >= 0 && x < _width);
        assert(y >= 0 && y < _height);
        return _pixels[y * _width + x];
    }

    int getWidth()  const { return _width; }
    int getHeight() const { return _height; }
    float getAspectRatio() const {
        assert(_height > 0);
        return static_cast<float>(_width) / _height;
    }

private:
    int _width;
    int _height;
    std::vector<Color> _pixels;
};
