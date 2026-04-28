#include "Renderer.h"
#include "glm/geometric.hpp"

Color Renderer::traceRay(const Ray& r, int depth) {
    if(depth >= maxDepth) {
        return Color(0, 0, 0);
    }

    HitInfo hitInfo;
    if (world->getScene()->intersect(r, 0.001f, 1000.0f, hitInfo)) {
        return computeShading(r, hitInfo, depth);
    }

    return backgroundColor;

    // Skybox
    /*
    glm::vec3 unit_direction = glm::normalize(r.direction());
    float a = 0.5*(unit_direction.y + 1.0);
    return (1.0f-a)*Color(1.0, 1.0, 1.0) + a*Color(0.5, 0.7, 1.0);
    */
}

Color Renderer::computeShading(const Ray& r, HitInfo hit, int depth)
{
    Color color = Color();

    // Luz ambiental
    color += Color(0.1, 0.1, 0.1) * hit.material->getAlbedo();

    // Luz directa
    for(auto light : world->getLights()) {
        if(light->castsShadows()) {
            Ray shadowRay = Ray(hit.p, light->getShadowDir(hit.p));
            if (world->getScene()->intersect(shadowRay, 0.001f, glm::length(light->getPosOrDir() - hit.p))) {
                continue;
            }
        }
        color += light->shade(r, hit);
    }

    // Reflexiones
    if(hit.material->getReflectance() > 0.0f) {
        glm::vec3 reflectDir = glm::reflect(r.direction(), hit.normal);
        Ray reflectRay(hit.p, reflectDir);
        color += hit.material->getReflectance() * traceRay(reflectRay, depth + 1);
    }

    return color;
}

void Renderer::render() {
    const int height = film.getHeight();
    const int width = film.getWidth();

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int depth = 0;
            const Ray ray_primary = camera.getRay(x, y);        // Generar rayo primario desde la cámara
            const Color c = traceRay(ray_primary, depth);       // Intersectar con la escena y calcular el color
            film.setPixel(x, y, c);                             // Escribir el color en el film
        }
    }
}
