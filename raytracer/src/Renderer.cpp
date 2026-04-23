#include "Renderer.h"
#include "glm/geometric.hpp"

Color Renderer::ray_color(const Ray& r) {
    HitInfo hitInfo;
    if (world->getScene()->intersect(r, 0.0, 10.0, hitInfo)) {
        return shade(r, hitInfo);
    }

    // Background color
    return BLACK;
    
    // Skybox
    /*
    glm::vec3 unit_direction = glm::normalize(r.direction());
    float a = 0.5*(unit_direction.y + 1.0);
    return (1.0f-a)*Color(1.0, 1.0, 1.0) + a*Color(0.5, 0.7, 1.0);
    */
}

Color Renderer::shade(Ray r, HitInfo hit) {
    Color ret;
    // Ambiente
    ret += Color(0.1, 0.1, 0.1);

    // Luces
    for(auto light : world->getLights()) 
        ret += light->shade(r, hit);
    
    return ret;
}

void Renderer::render() {
    for (std::size_t y = 0; y < film.GetTamY(); ++y) {
        for (std::size_t x = 0; x < film.GetTamX(); ++x) {
            const Ray ray_primary = camera.get_ray(x, y);
            const Color c = ray_color(ray_primary);
            film.AddPixel(c);
        }
    }
}