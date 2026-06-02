#pragma once

#include "DemoScene.h"
#include "DemoIncludes.h"

class Demo3 : public DemoScene
{
private:
    Film film;
    Camera cam;
    std::shared_ptr<World> world;


    // Textures
    std::shared_ptr<ConstantTexture> whiteTexture;
    std::shared_ptr<ConstantTexture> redTexture;
    std::shared_ptr<ConstantTexture> greenTexture;

    std::shared_ptr<ImageTexture> earthAlbedo;
    std::shared_ptr<ImageTexture> earthNormal;
    std::shared_ptr<ImageTexture> earthRough;
    std::shared_ptr<ConstantTexture> earthMetal;

    // Materials
    std::shared_ptr<Material> whiteWall;
    std::shared_ptr<Material> redWall;
    std::shared_ptr<Material> greenWall;

    std::shared_ptr<Material> goldSphere;
    std::shared_ptr<Material> chromeSphere;

    std::shared_ptr<CookTorranceMaterial> earthSphere;


public:


    Demo3()
    : DemoScene("Demo3")
    , film(1920,1080)
    , cam(
        {0.0f, 0.7f, 2.30f},
        {0.0f, 0.3f, -4.0f},
        {0.0f, 1.0f, 0.0f},
        film,
        45.0f,
        0.0f,
        8.0f)
    {}

    ~Demo3() = default;

    void loadTextures() override
    {
        whiteTexture =
            ConstantTexture::createTexture(
                Color(0.8f,0.8f,0.8f));

        redTexture =
            ConstantTexture::createTexture(
                Color(0.8f,0.1f,0.1f));

        greenTexture =
            ConstantTexture::createTexture(
                Color(0.1f,0.8f,0.1f));

        earthAlbedo =
            ImageTexture::createTexture(
                "8081_earthmap10k.jpg");

        earthNormal =
            ImageTexture::createTexture(
                "earth_normal_map.jpg");

        earthRough =
            ImageTexture::createTexture(
                "8081_earthspec10k.jpg");

        earthMetal =
            ConstantTexture::createTexture(
                Color(0.0f));
    }

    void loadMaterials() override
    {
        whiteWall = std::make_shared<CookTorranceMaterial>(whiteTexture, 0.85f, 0.0f);

        redWall = std::make_shared<CookTorranceMaterial>(redTexture, 0.85f, 0.0f);

        greenWall = std::make_shared<CookTorranceMaterial>(greenTexture, 0.85f, 0.0f);

        goldSphere = std::make_shared<CookTorranceMaterial>(Color(1.0f,0.766f,0.336f), 0.05f, 1.0f);

        chromeSphere = std::make_shared<CookTorranceMaterial>(Color(0.95f,0.95f,0.95f), 0.02f, 1.0f);

        earthSphere = std::make_shared<CookTorranceMaterial>(earthAlbedo, earthRough, earthMetal);
        earthSphere->setNormalMap(earthNormal);

    }

    void loadScene() override
    {
        auto scene = std::make_shared<Scene>();

        // Suelo
        scene->addShape(
            std::make_shared<Plane>(
                glm::vec3(-2.f,-1.f,0.f),
                glm::vec3( 4.f, 0.f,0.f),
                glm::vec3( 0.f, 0.f,-8.f),
                whiteWall
            )
        );

        // Techo
        scene->addShape(
            std::make_shared<Plane>(
                glm::vec3(-2.f,3.f,0.f),
                glm::vec3(4.f,0.f,0.f),
                glm::vec3(0.f,0.f,-8.f),
                whiteWall
            )
        );

        // Pared del fondo
        scene->addShape(
            std::make_shared<Plane>(
                glm::vec3(-2.f,-1.f,-8.f),
                glm::vec3(4.f,0.f,0.f),
                glm::vec3(0.f,4.f,0.f),
                whiteWall
            )
        );

        // Pared izda
        scene->addShape(
            std::make_shared<Plane>(
                glm::vec3(-2.f,-1.f,0.f),
                glm::vec3(0.f,0.f,-8.f),
                glm::vec3(0.f,4.f,0.f),
                redWall
            )
        );

        // Pared dcha
        scene->addShape(
            std::make_shared<Plane>(
                glm::vec3(2.f,-1.f,0.f),
                glm::vec3(0.f,0.f,-8.f),
                glm::vec3(0.f,4.f,0.f),
                greenWall
            )
        );

        // Esferas
        scene->addShape(
            std::make_shared<Sphere>(
                glm::vec3(-0.9f,-0.2f,-5.0f),
                0.8f,
                goldSphere
            )
        );

        scene->addShape(
            std::make_shared<Sphere>(
                glm::vec3(0.9f,-0.2f,-4.0f),
                0.8f,
                chromeSphere
            )
        );

        scene->addShape(
            std::make_shared<Sphere>(
                glm::vec3(0.0f,1.0f,-6.0f),
                0.5f,
                earthSphere
            )
        );

        world = std::make_shared<World>(scene);

        world->addLight(std::make_shared<PointLight>(glm::vec3(0.0f,2.7f,-4.0f), WHITE, 20.f));
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
