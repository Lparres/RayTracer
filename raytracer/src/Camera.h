#pragma once

#include "glm/vec3.hpp"
#include "Film.h"
#include "Ray.h"

class Camera {

public:
    Camera(
        glm::vec3 position,
        glm::vec3 look,
        glm::vec3 up,
        const Film &film,
        const float fov_degrees_vertical
    );

    Ray get_ray(int x, int y) const;

private:
    glm::vec3 position;
    glm::vec3 delta_x;
    glm::vec3 delta_y;
    glm::vec3 position_top_left;

};