#include "Renderer.h"
#include "glm/geometric.hpp"

Color Renderer::ray_color(const Ray& r, int& bounceCount) {
    if(bounceCount > MAX_BOUNCES) {
        return Color(0, 0, 0);
    }

    HitInfo hitInfo;
    if (world->getScene()->intersect(r, 0.001f, 1000.0f, hitInfo)) {
        return shade(r, hitInfo, ++bounceCount);
    }

    // Background color
    return BACKGROUND_COLOR;

    // Skybox
    /*
    glm::vec3 unit_direction = glm::normalize(r.direction());
    float a = 0.5*(unit_direction.y + 1.0);
    return (1.0f-a)*Color(1.0, 1.0, 1.0) + a*Color(0.5, 0.7, 1.0);
    */
}

Color Renderer::shade(Ray r, HitInfo hit, int& bounceCount) {
    Color ret = Color();
    // Ambiente
    ret += Color(0.1, 0.1, 0.1) * hit.material->get_albedo();

    // Luces
    for(auto light : world->getLights()) {
        if(light->castsShadows()) {
            Ray shadowRay = Ray(hit.p, light->getShadowDir(hit.p));
            if (world->getScene()->intersect(shadowRay, 0.001f, glm::length(light->getPosOrDir() - hit.p))) {
                continue;
            }
        }
        ret += light->shade(r, hit);
    }

    if(hit.material->get_reflectance() > 0.f) {
        glm::vec3 reflectDir = glm::reflect(r.direction(), hit.normal);
        Ray reflectRay(hit.p, reflectDir);
        ret += hit.material->get_reflectance() * ray_color(reflectRay, bounceCount);
    }

    return ret;
}

void Renderer::render() {
    const int height = film.getHeight();
    const int width = film.getWidth();

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int bounceCount = 0;
            const Ray ray_primary = camera.get_ray(x, y);   // Generar rayo primario desde la cámara
            const Color c = ray_color(ray_primary, bounceCount);         // Intersectar con la escena y calcular el color
            film.setPixel(x, y, c);                         // Escribir el color en el film
        }
    }
}
