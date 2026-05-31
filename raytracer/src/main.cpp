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
#include "GradientEnvironment.h"
#include "HDRIEnvironment.h"
#include "CubemapEnvironment.h"

#include <chrono>
#include <iomanip>
#include <fstream>

int main(void)
{
    Film film{1920, 1080};

    std::cout << "Cargando texturas...\n";
    const auto texturesStartTime = std::chrono::steady_clock::now();

    // Carga de texturas
    std::shared_ptr<ConstantTexture> verdeTexture = ConstantTexture::createTexture(GREEN);
    std::shared_ptr<ConstantTexture> azulTexture = ConstantTexture::createTexture(BLUE);
    std::shared_ptr<ConstantTexture> whiteTexture = ConstantTexture::createTexture(WHITE);
    std::shared_ptr<ImageTexture> maderaTexture = ImageTexture::createTexture("wood.png");
    std::shared_ptr<CheckerTexture> patron1Texture = CheckerTexture::createTexture(verdeTexture, maderaTexture, 2, 2);
    std::shared_ptr<CheckerTexture> patron2Texture = CheckerTexture::createTexture(whiteTexture, maderaTexture, 2, 2);
    std::shared_ptr<CheckerTexture> patronFinalTexture = CheckerTexture::createTexture(patron1Texture, patron2Texture, 6, 8);
    std::shared_ptr<ImageTexture> earthAlbedo = ImageTexture::createTexture("8081_earthmap10k.jpg");
    std::shared_ptr<ImageTexture> earthNormal = ImageTexture::createTexture("earth_normal_map.jpg");
    std::shared_ptr<ImageTexture> earthRough  = ImageTexture::createTexture("8081_earthspec10k.jpg");
    std::shared_ptr<ConstantTexture> earthMetal = ConstantTexture::createTexture(Color(0.0f));
    std::shared_ptr<ImageTexture> environmentMap = ImageTexture::createTexture("meadow.hdr");
    /*
    std::shared_ptr<ImageTexture> cubemap_top = ImageTexture::createTexture("skybox/top.jpg");
    std::shared_ptr<ImageTexture> cubemap_left = ImageTexture::createTexture("skybox/left.jpg");
    std::shared_ptr<ImageTexture> cubemap_front = ImageTexture::createTexture("skybox/front.jpg");
    std::shared_ptr<ImageTexture> cubemap_right = ImageTexture::createTexture("skybox/right.jpg");
    std::shared_ptr<ImageTexture> cubemap_back = ImageTexture::createTexture("skybox/back.jpg");
    std::shared_ptr<ImageTexture> cubemap_bottom = ImageTexture::createTexture("skybox/bottom.jpg");
    */

    const auto texturesEndTime = std::chrono::steady_clock::now();
    const std::chrono::duration<double> texturesElapsedSeconds = texturesEndTime - texturesStartTime;
    std::cout << std::fixed << std::setprecision(2)
              << "Tiempo de carga de texturas: " << texturesElapsedSeconds.count() << " s\n";

    std::cout << "Cargando materiales...\n";
    const auto materialsStartTime = std::chrono::steady_clock::now();

    // Creación de materiales
    std::shared_ptr<Material> azul = std::make_shared<BlinnPhongMaterial>(BLUE, 60.0f, 0.5f);
    // Oro
    const Color GOLD = Color(1.0f, 0.766f, 0.336f);
    std::shared_ptr<Material> amarillo = std::make_shared<CookTorranceMaterial>(GOLD, 0.4f, 1.0f);
    std::shared_ptr<Material> rojo = std::make_shared<CookTorranceMaterial>(RED, 0.08f, 0.0f);
    std::shared_ptr<Material> verde = std::make_shared<CookTorranceMaterial>(GREEN, 0.5f, 0.0f);
    std::shared_ptr<Material> sueloTexturizado = std::make_shared<CookTorranceMaterial>(patronFinalTexture, 0.3f, 0.0f);
    std::shared_ptr<Material> madera = std::make_shared<CookTorranceMaterial>(maderaTexture, 0.7f, 0.0f);

    auto earth = std::make_shared<CookTorranceMaterial>(
        earthAlbedo,
        earthRough,
        earthMetal
    );
    earth->setNormalMap(earthNormal);

    const auto materialsEndTime = std::chrono::steady_clock::now();
    const std::chrono::duration<double> materialsElapsedSeconds = materialsEndTime - materialsStartTime;
    std::cout << std::fixed << std::setprecision(2)
              << "Tiempo de creacion de materiales: " << materialsElapsedSeconds.count() << " s\n";

    std::cout << "Instanciando escena...\n";
    const auto sceneStartTime = std::chrono::steady_clock::now();

    // Instanciación de la escena
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

    std::shared_ptr<Light> pointLight = std::make_shared<PointLight>(glm::vec3{2.0f, 2.0f, 2.0f}, WHITE, 30.0f);
    world->addLight(pointLight);

    // std::shared_ptr<Environment> enviro = std::make_shared<GradientEnvironment>();
    std::shared_ptr<Environment> enviro = std::make_shared<HDRIEnvironment>(environmentMap);
    // std::shared_ptr<Environment> enviro = std::make_shared<CubemapEnvironment>(
    //     cubemap_top, cubemap_left, cubemap_front, cubemap_right, cubemap_back, cubemap_bottom
    // );
    world->setEnvironment(enviro);

    const Camera cam{
        {0.0, 0.0, 3.0},
        {0.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        film,
        60.0f,
        0.f,
        5.0f
    };

    const auto sceneEndTime = std::chrono::steady_clock::now();
    const std::chrono::duration<double> sceneElapsedSeconds = sceneEndTime - sceneStartTime;
    std::cout << std::fixed << std::setprecision(2)
              << "Tiempo de instanciacion de escena: " << sceneElapsedSeconds.count() << " s\n";

    Renderer renderer(film, cam, world);
    renderer.render();

    // Exportamos el resultado a un archivo PPM
    std::cout << "Exportando resultado...\n";
    const auto writeStart = std::chrono::steady_clock::now();
    FilmWriter::writePPM(film, "intensidad30.ppm", true);
    const auto writeEnd = std::chrono::steady_clock::now();
    const std::chrono::duration<double> writeElapsed = writeEnd - writeStart;
    std::cout << std::fixed << std::setprecision(2)
              << "Tiempo de exportacion PPM: " << writeElapsed.count() << " s\n";

    return 0;
}
