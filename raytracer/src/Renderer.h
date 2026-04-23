#pragma once
#include "Ray.h"
#include "Camera.h"
#include "Film.h"
#include "World.h"
#include <memory>

class Renderer {
public:

    Renderer(const Film& film, const Camera& camera, std::shared_ptr<World> world) : film(film), camera(camera), world(world) {}
    ~Renderer() = default;

    void render();

private:
    Color ray_color(const Ray& r);
    Color shade(Ray r, HitInfo hit);

private:
    Film film;
    Camera camera;
    std::shared_ptr<World> world;
};
