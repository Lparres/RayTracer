#include "glm/vec3.hpp"
#include "glm/geometric.hpp"
#include "Film.h"
#include "Color.h"
#include "Camera.h"
#include "Sphere.h"
#include "Renderer.h"
#include "Scene.h"
#include "DirectionalLight.h"
#include "PointLight.h"

#include <fstream>

int main(void) {
    std::ofstream out{"imagen.ppm"};
    Film film{1920, 1080, out};
    glm::vec3 unit_direction{};

    std::shared_ptr<Material> azul = std::make_shared<Material>(BLUE);
    std::shared_ptr<Material> amarillo = std::make_shared<Material>(YELLOW);
    std::shared_ptr<Material> rojo = std::make_shared<Material>(RED);
    std::shared_ptr<Material> verde = std::make_shared<Material>(GREEN);

    std::shared_ptr<Sphere> s1 = std::make_shared<Sphere>(glm::vec3(-2, 0, -2), 1.0, rojo);
    std::shared_ptr<Sphere> s2 = std::make_shared<Sphere>(glm::vec3(0, 0, -2), 1.0, amarillo);
    std::shared_ptr<Sphere> s3 = std::make_shared<Sphere>(glm::vec3(2, 0, -2), 1.0, azul);
    std::shared_ptr<Sphere> s4 = std::make_shared<Sphere>(glm::vec3(0, -100, -2), 99.0, verde);

    std::shared_ptr<Scene> scene = std::make_shared<Scene>();
    scene->addShape(s1);
    scene->addShape(s2);
    scene->addShape(s3);
    scene->addShape(s4);

    std::shared_ptr<World> world = std::make_shared<World>(scene);

    std::shared_ptr<Light> directionalLight = std::make_shared<DirectionalLight>(glm::vec3{-1,-1,0}, WHITE);
    world->addLight(directionalLight);

    std::shared_ptr<Light> pointLight = std::make_shared<PointLight>(glm::vec3{-2,2,3}, WHITE);
    world->addLight(pointLight);

    const Camera cam{
        {0.0, 0.0, 3.0},
        {0.0, 0.0, .0},
        {0.0, 1.0, 0.0},
        film,
        60.0
    };

    Renderer renderer(film, cam, world);
    renderer.render();

    return 0;
}
