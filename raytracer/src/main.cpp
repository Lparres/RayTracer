#include "glm/vec3.hpp"
#include "glm/geometric.hpp"
#include "Film.h"
#include "Color.h"
#include "Camera.h"
#include "Sphere.h"
#include "Renderer.h"
#include "Scene.h"

#include <fstream>

int main(void) {
    std::ofstream out{"imagen.ppm"};
    Film film{1920, 1080, out};
    glm::vec3 unit_direction{};

    std::shared_ptr<Material> azul = std::make_shared<Material>(BLUE);
    std::shared_ptr<Material> amarillo = std::make_shared<Material>(YELLOW);
    std::shared_ptr<Material> rojo = std::make_shared<Material>(RED);

    std::shared_ptr<Sphere> obj3 = std::make_shared<Sphere>(glm::vec3(-1, 0, -1), 0.5, azul);
    std::shared_ptr<Sphere> obj2 = std::make_shared<Sphere>(glm::vec3(0, 0, -2), 1.0, amarillo);
    std::shared_ptr<Sphere> obj1 = std::make_shared<Sphere>(glm::vec3(1, 0, -1), 0.5, rojo);

    std::shared_ptr<Scene> scene = std::make_shared<Scene>();
    scene->addShape(obj1);
    scene->addShape(obj2);
    scene->addShape(obj3);

    const Camera cam{
        {0.0, 0.0, 0.0},
        obj2->get_center(),
        {0.0, 1.0, 0.0},
        film,
        90.0
    };

    Renderer renderer(film, cam, scene);
    renderer.render();

    return 0;
}
