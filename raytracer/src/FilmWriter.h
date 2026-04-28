#pragma once
 
#include "Film.h"
#include <string>
#include <ostream>
 
// FilmWriter se encarga de escribir el contenido de un Film a un archivo o stream en formato PPM P3
class FilmWriter {
public:
    static void writePPM(const Film& film, std::ostream& out);
    static void writePPM(const Film& film, const std::string& path);
 
private:
    // Convierte un canal en espacio lineal a un valor corregido gamma
    static int linearToGamma(float channel);
};