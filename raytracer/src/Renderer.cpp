#include "Renderer.h"
#include "glm/geometric.hpp"

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

Color Renderer::traceRay(const Ray& incomingRay, int currentDepth)
{
    // Límite de profundidad para evitar recursión infinita en reflexiones
    if(currentDepth >= maxDepth) {
        return Color(0, 0, 0);
    }

    // Intersectamos el rayo con la escena
    HitInfo hitInfo;
    if (world->getScene()->intersect(incomingRay, 0.001f, 1000.0f, hitInfo)) {
        return computeShading(incomingRay, hitInfo, currentDepth);
    }

    // Si no hay intersección, muestreamos el color del entorno
    return sampleEnvironment(incomingRay);
}

Color Renderer::computeShading(const Ray& incomingRay, const HitInfo& hitInfo, int currentDepth)
{
    Color color = Color();

    // Luz ambiental
    color += Color(0.1, 0.1, 0.1) * hitInfo.material->getAlbedo();

    // Luz directa
    for(const auto& light : world->getLights()) {
        if(light->castsShadows()) {
            const Light::ShadowRay shadowRay = light->getShadowRay(hitInfo.p);
            if (world->getScene()->intersect(shadowRay.ray, 0.001f, shadowRay.maxDistance)) {
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

Color Renderer::sampleEnvironment(const Ray& incomingRay) const
{

    return backgroundColor;

    // Skybox
    /*
    glm::vec3 unitDirection = glm::normalize(incomingRay.direction());
    float a = 0.5f * (unitDirection.y + 1.0f);
    return (1.0f - a) * Color(1.0f, 1.0f, 1.0f) + a * Color(0.5f, 0.7f, 1.0f);
    */
}
