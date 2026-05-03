#include "Camera.h"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"

#include <stdexcept>


Camera::Camera(
    glm::vec3 position,
    glm::vec3 look,
    glm::vec3 up,
    const Film &film,
    const float fov_degrees_vertical,
    float focusAngle,
    float focusDistance
) : 
    position(position),
    focusAngle(focusAngle),
    focusDistance(focusDistance),
    gen(rd()),
    dist(-1.0f, 1.0f)
    {
    if (film.getWidth() <= 0 || film.getHeight() <= 0) {
        throw std::invalid_argument("Film dimensions must be positive");
    }

    if(focusDistance <= 0.0f) {
        throw std::invalid_argument("Focus distance must be positive");
    }

    const float fov_radians_vertical = glm::radians(fov_degrees_vertical * 0.5f);
    const float half_height = std::tan(fov_radians_vertical);

    if (glm::length(position - look) <= 0.0f) {
        throw std::invalid_argument("Camera position and look point cannot coincide");
    }
    forward = glm::normalize(position - look);

    right = glm::cross(up, forward);
    const float right_length = glm::length(right);
    if (right_length <= 0.0f) {
        throw std::invalid_argument("Camera up vector cannot be parallel to the view direction");
    }
    right /= right_length;

    up = glm::normalize(glm::cross(forward, right));

    const float half_height_viewport = focusDistance * half_height;
    const float half_width_viewport = half_height_viewport * film.getAspectRatio();

    const float height_viewport = half_height_viewport * 2.0f;
    const float width_viewport = half_width_viewport * 2.0f;

    const float pixel_height = height_viewport / float(film.getHeight());
    const float pixel_width = width_viewport / float(film.getWidth());

    delta_x = right * pixel_width;
    delta_y = -up * pixel_height;
    position_top_left =
        position - focusDistance * forward
        + up * half_height_viewport + delta_x * 0.5f
        - right * half_width_viewport + delta_y * 0.5f;

    const float blurRadius = focusDistance * glm::tan( glm::radians(focusAngle) / 2.0f );

    defocus_right = right * blurRadius;
    defocus_up = up * blurRadius;
    
}

Ray Camera::getRay(int x, int y) const {
    std::pair<float, float> sampleOffset = randomInSquare();
    const glm::vec3 sample = 
        position_top_left 
        + delta_x * (static_cast<float>(x) + sampleOffset.first)
        + delta_y * (static_cast<float>(y) + sampleOffset.second);

    std::pair<float,float> blur = randomInCircle();
    glm::vec3 origin = 
        focusAngle <= 0 ? position
        : position + blur.first * defocus_right + blur.second * defocus_up;

    glm::vec3 displacement = glm::normalize(sample - origin);

    return Ray{origin, displacement};
}

std::pair<float,float> Camera::randomInCircle() const {
    std::pair<float, float> point;

    do {
        point.first = dist(gen);
        point.second = dist(gen);
    } while (point.first * point.first + point.second * point.second > 1.0f);

    return point;
}

std::pair<float, float> Camera::randomInSquare() const {
    std::pair<float, float> point;

    point.first = dist(gen) - 0.5f;
    point.second = dist(gen) - 0.5f;

    return point;
}