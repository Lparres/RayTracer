#include "FilmWriter.h"
#include "glm/common.hpp"
 
#include <fstream>
#include <cmath>
#include <stdexcept>
 
int FilmWriter::linearToGamma(float channel, bool applyGamma) {
    // Clamp a [0,1]
    float clamped = glm::clamp(channel, 0.0f, 1.0f);
    if (!applyGamma) {
        return static_cast<int>(255.999f * clamped);
    }
    // Aplicar corrección gamma 
    float gamma_corrected = sqrt(clamped);
    return static_cast<int>(255.999f * gamma_corrected);
}

void FilmWriter::writePPM(const Film& film, std::ostream& out, bool applyGamma) {
    const int width  = film.getWidth();
    const int height = film.getHeight();
 
    // PPM P3 header
    out << "P3\n" << width << ' ' << height << "\n255\n";
 
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const Color c = film.getPixel(x, y);
            out << linearToGamma(c.r, applyGamma) << ' '
                << linearToGamma(c.g, applyGamma) << ' '
                << linearToGamma(c.b, applyGamma) << '\n';
        }
    }
}
 
void FilmWriter::writePPM(const Film& film, const std::string& path, bool applyGamma) {
    std::ofstream file(path);
    writePPM(film, file, applyGamma);
}