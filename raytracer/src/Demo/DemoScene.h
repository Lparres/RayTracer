#pragma once

#include <iostream>
#include <chrono>
#include <iomanip>
#include <fstream>

#include "Renderer.h"
#include "Film.h"
#include "Camera.h"
#include "FilmWriter.h"

class DemoScene {
public:
    virtual ~DemoScene() = default;

    virtual void loadTextures() = 0;
    virtual void loadMaterials() = 0;
    virtual void loadScene() = 0;
    virtual void renderScene() = 0;
    virtual void exportResult() = 0;

    std::string name;

protected:
    DemoScene(std::string name) : name(name) {}

    void exportFilm(const Film& film) {
        FilmWriter::writePPM(film, name + ".ppm", true);
    }
};