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
    _focusAngle(focusAngle),
    _focusDistance(focusDistance),
    gen(rd()),
    dist(-1.0f, 1.0f)
    {
    if (film.getWidth() <= 0 || film.getHeight() <= 0) {
        throw std::invalid_argument("Film dimensions must be positive");
    }

    const float fov_radians_vertical = glm::radians(fov_degrees_vertical * 0.5f);
    const float half_height_normalized = std::tan(fov_radians_vertical);

    const glm::vec3 forward_displacement = position - look;
    const float focal_length = glm::length(forward_displacement);
    if (focal_length <= 0.0f) {
        throw std::invalid_argument("Camera position and look point cannot coincide");
    }

    const glm::vec3 forward = forward_displacement / focal_length;
    glm::vec3 right = glm::cross(up, forward);
    const float right_length = glm::length(right);
    if (right_length <= 0.0f) {
        throw std::invalid_argument("Camera up vector cannot be parallel to the view direction");
    }
    right /= right_length;
    const glm::vec3 up_orthonormal = glm::normalize(glm::cross(forward, right));

    const float half_height_viewport = focal_length * half_height_normalized;
    const float half_width_viewport = half_height_viewport * film.getAspectRatio();

    const float height_viewport = half_height_viewport * 2.0f;
    const float width_viewport = half_width_viewport * 2.0f;

    const float pixel_height = height_viewport / float(film.getHeight());
    const float pixel_width = width_viewport / float(film.getWidth());

    delta_x = right * pixel_width;
    delta_y = -up_orthonormal * pixel_height;
    position_top_left =
        position - focal_length * forward
        + up_orthonormal * half_height_viewport + delta_x * 0.5f
        - right * half_width_viewport + delta_y * 0.5f;

    _blurRadius = _focusDistance * glm::tan( glm::radians(_focusAngle) / 2.0f );

    
    
}

Ray Camera::getRay(int x, int y) const {
    const glm::vec3 sample = position_top_left + delta_x * (float)x + delta_y * (float)y;

    glm::vec3 origen = position;

    origen.x += getRandomBlur();
    origen.y += getRandomBlur();

    glm::vec3 displacement = (sample - origen);

    return Ray{position, glm::normalize(displacement)};
}

float Camera::getRandomBlur() const
{
    float randomNum = dist(gen);
    return _blurRadius * randomNum;
}
