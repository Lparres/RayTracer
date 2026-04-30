#pragma once

#include "glm/vec3.hpp"
#include "Film.h"
#include "Ray.h"
#include <random>

// Camera es la responsable de la proyección
// Precalcula todos los vectores necesarios para generar rayos
// Expone el método getRay(x, y) para devolver el rayo primario correspondiente a un píxel de la imagen.
class Camera {

public:
    Camera(
        glm::vec3 position,
        glm::vec3 look,
        glm::vec3 up,
        const Film &film,
        const float fov_degrees_vertical,
        float focusAngle,
        float focusDistance
    );

    Ray getRay(int x, int y) const;


private:

    float getRandomBlur() const;

    glm::vec3 position;
    glm::vec3 delta_x;
    glm::vec3 delta_y;
    glm::vec3 position_top_left;

    // Lens properties
    float _focusAngle;
    float _focusDistance;

    // Internal precomputed
    float _blurRadius;
    std::random_device rd;
    mutable std::mt19937 gen;
    mutable std::uniform_real_distribution<float> dist;
};
