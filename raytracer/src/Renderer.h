#pragma once
#include "Ray.h"
#include "Camera.h"
#include "Film.h"
#include "Scene.h"
#include <memory>

class Renderer {
public:

    Renderer(const Film& film, const Camera& camera, std::shared_ptr<Scene> scene) : film(film), camera(camera), scene(scene) {}
    ~Renderer() = default;

    void render();

private:
    Color ray_color(const Ray& r);

private:
    Film film;
    Camera camera;
    std::shared_ptr<Scene> scene;
};
