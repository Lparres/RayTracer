#include "Renderer.h"
#include "glm/geometric.hpp"
#include <chrono>
#include <iomanip>
#include <iostream>

void Renderer::render() {
    const auto startTime = std::chrono::steady_clock::now();
    const int height = film.getHeight();
    const int width = film.getWidth();

    // Para trackear el progreso
    int totalSteps = 10;
    int progressStep = height / totalSteps;

    for (int y = 0; y < height; ++y) {
        if(y % progressStep == 0) std::cout << "Renderizando... " << y * 100 / height << "%\n";
        for (int x = 0; x < width; ++x) {
            Color c = BLACK;
            for(int i = 0; i < samples; ++i) {
                const Ray rayPrimary = camera.getRay(x, y);    // Generar rayo primario desde la cámara
                c += traceRay(rayPrimary, 0);       // Intersectar con la escena y calcular el color
            }
            film.setPixel(x, y, c * samplesInv);    // Escribir el color en el film
        }
    }
    std::cout << "Terminado :)\n";

    const auto endTime = std::chrono::steady_clock::now();
    const std::chrono::duration<double> elapsedSeconds = endTime - startTime;
    std::cout << std::fixed << std::setprecision(2)
              << "Tiempo de render: " << elapsedSeconds.count() << " s\n";
}

Color Renderer::traceRay(const Ray& incomingRay, int currentDepth)
{
    // Límite de profundidad para evitar recursión infinita en reflexiones
    if(currentDepth >= maxDepth) {
        return Color(0, 0, 0);
    }

    // Intersectamos el rayo con la escena
    HitInfo hit;
    if (world->getScene().intersect(incomingRay, 0.001f, 1000.0f, hit)) {

        hit.normal = hit.material->shadingNormal(hit);

        // Iluminación directa
        Color direct = computeShading(incomingRay, hit);

        // Iluminación indirecta (rayos reflejados)
        Ray scattered;
        Color attenuation;

        if(hit.material->scatter(incomingRay, hit, attenuation, scattered))
            direct += attenuation * traceRay(scattered, currentDepth + 1);

        return direct;
    }

    // Si no hay intersección, muestreamos el color del entorno
    return sampleEnvironment(incomingRay);
}

Color Renderer::computeShading(const Ray& incomingRay, const HitInfo& hit)
{
    Color Lo = Color();

    // Esto no funciona porque evalúa especular sin querer, generando reflejos falsos.
    //Creo que hay que hacer un evaluateAmbient que en lugar de evaluar el lóbulo especular GGX,
    // usa F0 directamente como aporximación de la luz ambiental esepcular
    // Pero bueno por ahora lo dejo así
    glm::vec3 wi_ambient = hit.normal;
    Color f_ambient = hit.material->evaluateDirect(wi_ambient, -incomingRay.direction(), hit);
    Lo += Color(0.1f) * f_ambient * hit.material->ambientOcclusion(hit.uv);

    // Esta forma de evaluar luz ambiente no es correcta en
    //Lo += Color(0.1, 0.1, 0.1) * hit.material->albedo(hit.uv) * hit.material->ambientOcclusion(hit.uv);

    // Luz directa
    for (const auto& light : world->getLights())
    {
        auto [shadowRay, wi, Li] = light->getLightContribution(hit.p);

        if (light->castsShadows() && world->getScene().intersect(shadowRay.ray, 0.001f, shadowRay.maxDistance))
            continue;

        // BRDF
        Color f = hit.material->evaluateDirect(wi, -incomingRay.direction(), hit);

        float lambert = std::max(glm::dot(hit.normal, wi), 0.f);

        // Ecuación de renderizado:
        Lo += Li * f * lambert;
    }

    return Lo;
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
