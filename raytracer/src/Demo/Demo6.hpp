#pragma once

#include "DemoScene.h"
#include "DemoIncludes.h"

class Demo6 : public DemoScene
{
private:
    Film film;
    Camera cam;
    std::shared_ptr<World> world;


    std::shared_ptr<ImageTexture> environmentMap;

    std::shared_ptr<Material> gold;
    std::shared_ptr<Material> chrome;
    std::shared_ptr<Material> redPlastic;


public:
    Demo6()
    : DemoScene("Demo6")
    , film(1920,1080)
    , cam(
        {0.0f,1.5f,7.0f},
        {0.0f,0.0f,-4.0f},
        {0.0f,1.0f,0.0f},
        film,
        45.0f,
        0.0f,
        10.0f)
    {}

    ~Demo6() = default;

    void loadTextures() override
    {
        environmentMap = ImageTexture::createTexture("studio.hdr");
    }

    void loadMaterials() override
    {
        gold = std::make_shared<CookTorranceMaterial>(Color(1.0f,0.766f,0.336f), 0.05f, 1.0f);

        chrome = std::make_shared<CookTorranceMaterial>(Color(0.95f,0.95f,0.95f), 0.02f, 1.0f);

        redPlastic = std::make_shared<CookTorranceMaterial>(Color(0.8f,0.05f,0.05f),0.3f,0.0f);
    }

    void loadScene() override
    {
        auto scene = std::make_shared<Scene>();

        scene->addShape(
            std::make_shared<Sphere>(
                glm::vec3(-3.5f,0.0f,-4.0f),
                1.5f,
                gold
            )
        );

        scene->addShape(
            std::make_shared<Sphere>(
                glm::vec3(0.0f,0.0f,-5.0f),
                1.5f,
                chrome
            )
        );

        scene->addShape(
            std::make_shared<Sphere>(
                glm::vec3(3.5f,0.0f,-6.0f),
                1.5f,
                redPlastic
            )
        );

        world = std::make_shared<World>(scene);

        auto env = std::make_shared<HDRIEnvironment>(environmentMap);
        env->setIntensity(1.5f);

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
