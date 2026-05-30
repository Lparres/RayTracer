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
#include "ImageTexture.h"
#include "BlinnPhongMaterial.h"
#include "CookTorranceMaterial.h"

#include <fstream>

int main(void)
{
    Film film{1920, 1080};

    std::shared_ptr<ConstantTexture> verdeTexture = ConstantTexture::createTexture(GREEN);
    // std::shared_ptr<ConstantTexture> azulTexture = ConstantTexture::createTexture(BLUE);
    std::shared_ptr<ConstantTexture> whiteTexture = ConstantTexture::createTexture(WHITE);
    std::shared_ptr<ImageTexture> earthTexture = ImageTexture::createTexture("earth.jpg");
    std::shared_ptr<ImageTexture> maderaTexture = ImageTexture::createTexture("wood.png");
    std::shared_ptr<CheckerTexture> patron1Texture = CheckerTexture::createTexture(verdeTexture, maderaTexture, 2, 2);
    std::shared_ptr<CheckerTexture> patron2Texture = CheckerTexture::createTexture(whiteTexture, maderaTexture, 2, 2);
    std::shared_ptr<CheckerTexture> patronFinalTexture = CheckerTexture::createTexture(patron1Texture, patron2Texture, 6, 8);

    // std::shared_ptr<Material> azul = std::make_shared<Material>(BLUE, 60.0f, 0.5f);
    // Oro
    const Color GOLD = Color(1.0f, 0.766f, 0.336f);
    std::shared_ptr<Material> amarillo = std::make_shared<BlinnPhongMaterial>(GOLD, 0.8f, 1.0f);
    std::shared_ptr<Material> rojo = std::make_shared<BlinnPhongMaterial>(RED, 60.0f, 0.5f);
    std::shared_ptr<Material> verde = std::make_shared<BlinnPhongMaterial>(GREEN, 60.0f, 0.9f);
    std::shared_ptr<Material> sueloTexturizado = std::make_shared<BlinnPhongMaterial>(patronFinalTexture, 60.f, 0.9f);
    std::shared_ptr<Material> madera = std::make_shared<BlinnPhongMaterial>(maderaTexture);
    std::shared_ptr<Material> earth = std::make_shared<BlinnPhongMaterial>(earthTexture);


    std::shared_ptr<Sphere> s1 = std::make_shared<Sphere>(glm::vec3(-2.0f, 0.0f, -1.f), 1.0f, rojo);
    std::shared_ptr<Sphere> s2 = std::make_shared<Sphere>(glm::vec3(0.0f, 0.0f, -2.0f), 1.0f, earth);
    std::shared_ptr<Sphere> s3 = std::make_shared<Sphere>(glm::vec3(2.0f, 0.0f, -3.f), 1.0f, amarillo);
    // std::shared_ptr<Sphere> s4 = std::make_shared<Sphere>(glm::vec3(0, -100, -2), 99.0, verde);
    std::shared_ptr<Plane> s4 = std::make_shared<Plane>(glm::vec3(3.0f, -1.5f, 2.0f), glm::vec3(-6.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -6.0f), sueloTexturizado);

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
        60.0f,
        0.f,
        5.0f
    };

    Renderer renderer(film, cam, world);
    renderer.render();

    // Exportamos el resultado a un archivo PPM
    FilmWriter::writePPM(film, "imagen5BlinnPhong.ppm", true);

    return 0;
}
