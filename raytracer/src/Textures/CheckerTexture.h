#pragma once

#include <cmath>
#include <stdexcept>
#include "Texture.h"

// Textura que genera un patrón checkerboard a partir de dos texturas hijas.
//
// El patrón consiste en un grid de rows x columns.
// Cada celda individual delega el sample a la celda hija correspondiente (par o impar)
// normalizando las coordenadas UV a la celda local [0,1]x[0,1].
// Esto permite componer CheckerTextures anidadas a cualquier profundidad.
class CheckerTexture final : public Texture {
// Factoría
public:
    static std::shared_ptr<CheckerTexture> createTexture(
        std::shared_ptr<Texture> even, 
        std::shared_ptr<Texture> odd, 
        int rows, int columns
    ) {
        return std::make_shared<CheckerTexture>(Private(), even, odd, rows, columns);
    }

public:
    explicit CheckerTexture(Private p, std::shared_ptr<Texture> even, std::shared_ptr<Texture> odd, int rows, int columns)
        : _even(even)
        , _odd(odd)
        , _rows(rows)
        , _columns(columns)
    {
        if (rows <= 0 || columns <= 0)
            throw std::invalid_argument("CheckerTexture: rows and columns must be positive");
    }

    Color sample(UV uv) const override {
        const float wu = wrap(uv.u);
        const float wv = wrap(uv.v);

        const int col = cellIndex(wu, _columns);
        const int row = cellIndex(wv, _rows);

        const UV localUV = {
            wu * _columns - col,   // Posicion local dentro de la celda
            wv * _rows    - row
        };

        const bool isEven = (col + row) % 2 == 0;
        return isEven ? _even->sample(localUV) : _odd->sample(localUV);
    }

private:
    // Devuelve el índice de la celda para una coordenada en [0,1].
    static int cellIndex(float coord, int count) {
        const int i = static_cast<int>(std::floor(coord * count));
        return i < count ? i : count - 1;   // Ojo con coord == 1.0f
    }

    std::shared_ptr<Texture> _even;
    std::shared_ptr<Texture> _odd;
    int _rows;
    int _columns;
};
