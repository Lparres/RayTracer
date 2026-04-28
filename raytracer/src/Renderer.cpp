#include "Renderer.h"
#include "glm/geometric.hpp"

Color Renderer::traceRay(const Ray& incomingRay, int currentDepth) {
    if(currentDepth >= maxDepth) {
        return Color(0, 0, 0);
    }

    HitInfo hitInfo;
    if (world->getScene()->intersect(incomingRay, 0.001f, 1000.0f, hitInfo)) {
        return computeShading(incomingRay, hitInfo, currentDepth);
    }

    return backgroundColor;

    // Skybox
    /*
    glm::vec3 unit_direction = glm::normalize(r.direction());
    float a = 0.5*(unit_direction.y + 1.0);
    return (1.0f-a)*Color(1.0, 1.0, 1.0) + a*Color(0.5, 0.7, 1.0);
    */
}

Color Renderer::computeShading(const Ray& incomingRay, const HitInfo& hitInfo, int currentDepth)
{
    Color color = Color();

    // Luz ambiental
    color += Color(0.1, 0.1, 0.1) * hitInfo.material->getAlbedo();

    // Luz directa
    for(auto light : world->getLights()) {
        if(light->castsShadows()) {
            Ray shadowRay = Ray(hitInfo.p, light->getShadowDir(hitInfo.p));
            if (world->getScene()->intersect(shadowRay, 0.001f, glm::length(light->getPosOrDir() - hitInfo.p))) {
                continue;
            }
        }
        color += light->computeLighting(incomingRay, hitInfo);
    }

    // Reflexiones
    if(hitInfo.material->getReflectance() > 0.0f) {
        glm::vec3 reflectDir = glm::reflect(incomingRay.direction(), hitInfo.normal);
        Ray reflectRay(hitInfo.p, reflectDir);
        color += hitInfo.material->getReflectance() * traceRay(reflectRay, currentDepth + 1);
    }

    return color;
}

void Renderer::render() {
    const int height = film.getHeight();
    const int width = film.getWidth();

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const Ray rayPrimary = camera.getRay(x, y);    // Generar rayo primario desde la cámara
            const Color c = traceRay(rayPrimary, 0);       // Intersectar con la escena y calcular el color
            film.setPixel(x, y, c);                        // Escribir el color en el film
        }
    }
}
