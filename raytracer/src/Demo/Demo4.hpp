#pragma once

#include "DemoScene.h"
#include "DemoIncludes.h"

class Demo4 : public DemoScene
{
private:
    Film film;
    Camera cam;
    std::shared_ptr<World> world;


    // TEXTURES

    std::shared_ptr<ImageTexture> earthAlbedo;
    std::shared_ptr<ImageTexture> earthNormal;
    std::shared_ptr<ImageTexture> earthRough;
    std::shared_ptr<ConstantTexture> earthMetal;

    std::shared_ptr<ImageTexture> environmentMap;

    std::shared_ptr<ConstantTexture> groundTexture;

    // MATERIALS

    std::shared_ptr<CookTorranceMaterial> earth;

    std::shared_ptr<Material> gold;
    std::shared_ptr<Material> chrome;
    std::shared_ptr<Material> redPlastic;
    std::shared_ptr<Material> ground;


public:
    Demo4()
    : DemoScene("Demo4")
    , film(1920,1080)
    , cam(
        {5.0f, 2.0f, 8.0f},
        {0.0f, 0.4f, -5.0f},
        {0.0f, 1.0f, 0.0f},
        film,
        30.0f,
        1.8f,
        13.5f)
    {}

    ~Demo4() = default;

    void loadTextures() override
    {
        earthAlbedo = ImageTexture::createTexture("8081_earthmap10k.jpg");

        earthNormal = ImageTexture::createTexture("earth_normal_map.jpg");

        earthRough = ImageTexture::createTexture("8081_earthspec10k.jpg");

        earthMetal = ConstantTexture::createTexture(Color(0.0f));

        environmentMap = ImageTexture::createTexture("belfast_sunset_puresky.hdr");

        groundTexture = ConstantTexture::createTexture(Color(0.65f,0.65f,0.65f));
    }

    void loadMaterials() override
    {
        earth = std::make_shared<CookTorranceMaterial>(earthAlbedo, earthRough, earthMetal);
        earth->setNormalMap(earthNormal);

        gold = std::make_shared<CookTorranceMaterial>(Color(1.0f,0.766f,0.336f), 0.05f, 1.0f);

        chrome = std::make_shared<CookTorranceMaterial>(Color(0.95f,0.95f,0.95f), 0.02f, 1.0f);

        redPlastic = std::make_shared<CookTorranceMaterial>(Color(0.85f,0.05f,0.05f), 0.25f, 0.0f);

        ground = std::make_shared<CookTorranceMaterial>(groundTexture,0.55f,0.0f);
    }

    void loadScene() override
    {
        auto scene =
            std::make_shared<Scene>();

        scene->addShape(
            std::make_shared<Plane>(
                glm::vec3(-12.f,-1.5f,4.f),
                glm::vec3(24.f,0.f,0.f),
                glm::vec3(0.f,0.f,-30.f),
                ground
            )
        );

        scene->addShape(
            std::make_shared<Sphere>(
                glm::vec3(0.0f,0.3f,-5.5f),
                1.5f,
                earth
            )
        );

        scene->addShape(
            std::make_shared<Sphere>(
                glm::vec3(-3.0f,0.0f,-4.2f),
                1.0f,
                gold
            )
        );

        scene->addShape(
            std::make_shared<Sphere>(
                glm::vec3(3.0f,0.0f,-9.5f),
                1.0f,
                chrome
            )
        );

        scene->addShape(
            std::make_shared<Sphere>(
                glm::vec3(0.0f,-0.8f,-2.2f),
                0.45f,
                redPlastic
            )
        );

        world = std::make_shared<World>(scene);

        world->addLight(std::make_shared<DirectionalLight>(glm::vec3(-1.0f,-0.35f,-1.0f),Color(1.0f,0.92f,0.75f),4.0f));

        auto env =
            std::make_shared<HDRIEnvironment>(
                environmentMap);

        env->setIntensity(1.0f);

        world->setEnvironment(env);
    }

    void renderScene() override
    {
        Renderer renderer(film, cam, world);
        renderer.render();
    }

    void exportResult() override
    {
        DemoScene::exportFilm(film);
    }


};
