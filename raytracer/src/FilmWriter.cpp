#include "FilmWriter.h"
#include "glm/common.hpp"
 
#include <fstream>
#include <cmath>
#include <stdexcept>
 
int FilmWriter::linearToGamma(float channel) {
    // Clampeamos el canal a [0, 1], aplicamos corrección gamma (gamma 2.2) y escalamos a [0, 255].
    float clamped = glm::clamp(channel, 0.0f, 1.0f);
    float gamma_corrected = std::pow(clamped, 1.0f / 2.2f);
    return static_cast<int>(255.999f * gamma_corrected);
}

void FilmWriter::writePPM(const Film& film, std::ostream& out) {
    const int width  = film.getWidth();
    const int height = film.getHeight();
 
    // PPM P3 header
    out << "P3\n" << width << ' ' << height << "\n255\n";
 
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const Color c = film.getPixel(x, y);
            out << linearToGamma(c.r) << ' '
                << linearToGamma(c.g) << ' '
                << linearToGamma(c.b) << '\n';
        }
    }
}
 
void FilmWriter::writePPM(const Film& film, const std::string& path) {
    std::ofstream file(path);
    writePPM(film, file);
}