#include "Film.h"
#include "glm/geometric.hpp"

void Film::AddPixel(Color color) {
    if (missingHeader) {
        _out << "P3\n" << _tamX << ' ' << _tamY << "\n255\n";
        missingHeader = false;
    }

    int ir = static_cast<int>(255.999f * glm::clamp(color.r, 0.0f, 1.0f));
    int ig = static_cast<int>(255.999f * glm::clamp(color.g, 0.0f, 1.0f));
    int ib = static_cast<int>(255.999f * glm::clamp(color.b, 0.0f, 1.0f));

    _out << ir << ' ' << ig << ' ' << ib << '\n';
}
