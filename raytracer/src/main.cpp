#include "glm/vec3.hpp"
#include "glm/geometric.hpp"
#include "Film.h"
#include "FilmWriter.h"
#include "Color.h"
#include "Camera.h"
#include "Sphere.h"
#include "Plane.h"
#include "Renderer.h"
#include "Scene.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "ConstantTexture.h"
#include "CheckerTexture.h"

#include <fstream>

int main(void)
{
    Film film{1920, 1080};

    ConstantTexture gris(Color(0.25f, 0.25f, 0.25f));
    ConstantTexture azulTexture(BLUE);
    CheckerTexture patron1(gris, azulTexture, 4, 4);
    CheckerTexture patron2(ConstantTexture(1.0f, 1.0f, 1.0f), azulTexture, 4, 4);
    CheckerTexture patronFinal(patron1, patron2, 3, 4);

    std::shared_ptr<Material> azul = std::make_shared<Material>(BLUE, 30.0f, 0.5f);
    std::shared_ptr<Material> amarillo = std::make_shared<Material>(YELLOW, 30.0f, 0.9f);
    std::shared_ptr<Material> rojo = std::make_shared<Material>(RED, 30.0f, 0.5f);
    std::shared_ptr<Material> verde = std::make_shared<Material>(GREEN, 30.0f, 0.5f);
    std::shared_ptr<Material> sueloTexturizado = std::make_shared<Material>(patronFinal);


    std::shared_ptr<Sphere> s1 = std::make_shared<Sphere>(glm::vec3(-2.0f, 0.0f, -2.0f), 1.0f, rojo);
    std::shared_ptr<Sphere> s2 = std::make_shared<Sphere>(glm::vec3(0.0f, 0.0f, -2.0f), 1.0f, amarillo);
    std::shared_ptr<Sphere> s3 = std::make_shared<Sphere>(glm::vec3(2.0f, 0.0f, -2.0f), 1.0f, sueloTexturizado);
    // std::shared_ptr<Sphere> s4 = std::make_shared<Sphere>(glm::vec3(0, -100, -2), 99.0, verde);
    std::shared_ptr<Plane> s4 = std::make_shared<Plane>(glm::vec3(3.0f, -1.0f, 5.0f), glm::vec3(-6.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -6.0f), sueloTexturizado);

    std::shared_ptr<Scene> scene = std::make_shared<Scene>();
    scene->addShape(s1);
    scene->addShape(s2);
    scene->addShape(s3);
    scene->addShape(s4);

    std::shared_ptr<World> world = std::make_shared<World>(scene);

    std::shared_ptr<Light> pointLight = std::make_shared<PointLight>(glm::vec3{2.0f, 2.0f, 2.0f}, WHITE);
    world->addLight(pointLight);

    const Camera cam{
        {0.0, 0.0, 3.0},
        {0.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        film,
        60.0f
    };

    Renderer renderer(film, cam, world);
    renderer.render();

    // Exportamos el resultado a un archivo PPM
    FilmWriter::writePPM(film, "imagenConGamma.ppm", true);
    FilmWriter::writePPM(film, "imagenSinGamma.ppm", false);

    return 0;
}
