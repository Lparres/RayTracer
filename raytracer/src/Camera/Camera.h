#pragma once

#include "glm/vec3.hpp"
#include "Film.h"
#include "Ray.h"
#include <random>
#include <utility>

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
    // Devuelve un punto aleatorio en un círculo
    std::pair<float,float> randomInCircle() const;
    std::pair<float, float> randomInSquare() const;

    glm::vec3 position;
    glm::vec3 delta_x;
    glm::vec3 delta_y;
    glm::vec3 position_top_left;

    // Camera basis vectors
    glm::vec3 forward; // w
    glm::vec3 right; // u
    glm::vec3 up; // v

    // Lens properties
    float focusAngle;
    float focusDistance;

    // Internal precomputed
    glm::vec3 defocus_right;
    glm::vec3 defocus_up;

    mutable std::mt19937 gen;
    mutable std::uniform_real_distribution<float> dist;
};
