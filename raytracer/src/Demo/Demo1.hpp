#pragma once

#include "DemoScene.h"
#include "DemoIncludes.h"

class Demo1 : public DemoScene
{
private:
    Film film;
    Camera cam;
    std::shared_ptr<World> world;

    // Texturas
    std::shared_ptr<ConstantTexture> verdeTexture;
    std::shared_ptr<ConstantTexture> whiteTexture;
    std::shared_ptr<ImageTexture> maderaTexture;
    std::shared_ptr<CheckerTexture> patron1Texture;
    std::shared_ptr<CheckerTexture> patron2Texture;
    std::shared_ptr<CheckerTexture> patronFinalTexture ;
    std::shared_ptr<ImageTexture> earthAlbedo;
    std::shared_ptr<ImageTexture> earthNormal;
    std::shared_ptr<ImageTexture> earthRough;
    std::shared_ptr<ConstantTexture> earthMetal;
    std::shared_ptr<ImageTexture> environmentMap;

    // Materiales
    const Color GOLD = Color(1.0f, 0.766f, 0.336f);
    std::shared_ptr<Material> amarillo;
    std::shared_ptr<Material> rojo;
    std::shared_ptr<Material> verde;
    std::shared_ptr<Material> sueloTexturizado;
    std::shared_ptr<Material> madera;
    std::shared_ptr<CookTorranceMaterial> earth;


public:
    Demo1()
    : DemoScene("Demo1")
    , film(1920, 1080)
    , cam(
        {0.0, 0.0, 3.0},
        {0.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        film,
        60.0f,
        0.f,
        5.0f)
    {

    }
    ~Demo1() = default;

    void loadTextures() override {
        verdeTexture = ConstantTexture::createTexture(GREEN);
        whiteTexture = ConstantTexture::createTexture(WHITE);
        maderaTexture = ImageTexture::createTexture("wood.png");
        patron1Texture = CheckerTexture::createTexture(verdeTexture, maderaTexture, 2, 2);
        patron2Texture = CheckerTexture::createTexture(whiteTexture, maderaTexture, 2, 2);
        patronFinalTexture = CheckerTexture::createTexture(patron1Texture, patron2Texture, 6, 8);
        earthAlbedo = ImageTexture::createTexture("8081_earthmap10k.jpg");
        earthNormal = ImageTexture::createTexture("earth_normal_map.jpg");
        earthRough = ImageTexture::createTexture("8081_earthspec10k.jpg");
        earthMetal = ConstantTexture::createTexture(Color(0.0f));
        environmentMap = ImageTexture::createTexture("belfast_sunset_puresky.hdr");

    }

    void loadMaterials() override {
        amarillo = std::make_shared<CookTorranceMaterial>(GOLD, 0.05f, 1.0f);
        rojo = std::make_shared<CookTorranceMaterial>(RED, 0.08f, 0.0f);
        verde = std::make_shared<CookTorranceMaterial>(GREEN, 0.5f, 0.0f);
        sueloTexturizado = std::make_shared<CookTorranceMaterial>(patronFinalTexture, 0.3f, 0.0f);
        madera = std::make_shared<CookTorranceMaterial>(maderaTexture, 0.7f, 0.0f);

        earth = std::make_shared<CookTorranceMaterial>(
            earthAlbedo,
            earthRough,
            earthMetal);
        earth->setNormalMap(earthNormal);
    }

    void loadScene() override
    {
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

        world = std::make_shared<World>(scene);

        std::shared_ptr<Light> pointLight = std::make_shared<PointLight>(glm::vec3{2.0f, 2.0f, 2.0f}, WHITE, 30.0f);
        world->addLight(pointLight);

        std::shared_ptr<Environment> enviro = std::make_shared<HDRIEnvironment>(environmentMap);
        enviro->setIntensity(0.6f); // 60% de la intensidad original
        world->setEnvironment(enviro);
    }

    void renderScene() override
    {
        Renderer renderer(film, cam, world);
        renderer.render();
    }

    void exportResult() override {
        DemoScene::exportFilm(film);
    }
};