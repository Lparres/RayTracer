#pragma once
#include "Ray.h"
#include "Camera.h"
#include "Film.h"
#include "HitInfo.h"
#include "World.h"
#include <memory>

// Orquestador principal del raytracer. Contiene la lógica principal del renderizado:
//      - Generar rayos primarios desde la cámara
//      - Intersectar con la escena para obtener HitInfo
//      - Calcular el color de cada píxel usando las luces y materiales

class Renderer {
    static constexpr int maxDepth = 3;
    static constexpr int samples = 2;
    static constexpr float samplesInv = 1.f / samples; // double?
    const Color backgroundColor = BLACK;

public:

    Renderer(Film& film, const Camera& camera, std::shared_ptr<World> world) : film(film), camera(camera), world(world) {}
    ~Renderer() = default;

    void render();

private:
    Color traceRay(const Ray& incomingRay, int currentDepth);
    Color computeShading(const Ray& incomingRay, const HitInfo& hit);
    // Color sampleEnvironment(const Ray& incomingRay) const;

private:
    Film& film;
    const Camera& camera;
    std::shared_ptr<World> world;
};
