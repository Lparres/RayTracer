#pragma once

#include "glm/vec3.hpp"
#include "Film.h"
#include "Ray.h"

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
        const float fov_degrees_vertical
    );

    Ray getRay(int x, int y) const;

private:
    glm::vec3 position;
    glm::vec3 delta_x;
    glm::vec3 delta_y;
    glm::vec3 position_top_left;

};
