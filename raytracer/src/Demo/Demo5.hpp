#pragma once

#include "DemoScene.h"
#include "DemoIncludes.h"

class Demo5 : public DemoScene
{
private:
    Film film;
    Camera cam;
    std::shared_ptr<World> world;


    // Texturas
    std::shared_ptr<ImageTexture> woodAlbedo;
    std::shared_ptr<ImageTexture> woodNormal;
    std::shared_ptr<ImageTexture> woodRough;

    std::shared_ptr<ImageTexture> brickAlbedo;
    std::shared_ptr<ImageTexture> brickNormal;
    std::shared_ptr<ImageTexture> brickRough;

    std::shared_ptr<ImageTexture> marbleAlbedo;
    std::shared_ptr<ImageTexture> marbleNormal;
    std::shared_ptr<ImageTexture> marbleRough;

    std::shared_ptr<ImageTexture> concreteAlbedo;
    std::shared_ptr<ImageTexture> concreteNormal;
    std::shared_ptr<ImageTexture> concreteRough;

    std::shared_ptr<ConstantTexture> dielectricMetal;

    std::shared_ptr<ImageTexture> environmentMap;

    // Materiales
    std::shared_ptr<CookTorranceMaterial> wood;
    std::shared_ptr<CookTorranceMaterial> brick;
    std::shared_ptr<CookTorranceMaterial> marble;
    std::shared_ptr<CookTorranceMaterial> concrete;


public:
    Demo5()
    : DemoScene("Demo5")
    , film(1920,1080)
    , cam(
        {0.0f, 1.8f, 8.0f},
        {0.0f, 0.0f, -5.0f},
        {0.0f, 1.0f, 0.0f},
        film,
        35.0f,
        0.0f,
        10.0f)
    {}

    ~Demo5() = default;

    void loadTextures() override
    {
        woodAlbedo  = ImageTexture::createTexture("wood/wood_diffuse.jpg");
        woodNormal  = ImageTexture::createTexture("wood/wood_normal.jpg");
        woodRough   = ImageTexture::createTexture("wood/wood_roughness.jpg");

        brickAlbedo = ImageTexture::createTexture("brick/brick_diffuse.jpg");
        brickNormal = ImageTexture::createTexture("brick/brick_normal.jpg");
        brickRough  = ImageTexture::createTexture("brick/brick_roughness.jpg");

        marbleAlbedo = ImageTexture::createTexture("marble/marble_diffuse.jpg");
        marbleNormal = ImageTexture::createTexture("marble/marble_normal.jpg");
        marbleRough  = ImageTexture::createTexture("marble/marble_roughness.jpg");

        concreteAlbedo = ImageTexture::createTexture("concrete/concrete_diffuse.jpg");
        concreteNormal = ImageTexture::createTexture("concrete/concrete_normal.jpg");
        concreteRough  = ImageTexture::createTexture("concrete/concrete_roughness.jpg");

        dielectricMetal = ConstantTexture::createTexture(Color(0.0f));

        environmentMap = ImageTexture::createTexture("peppermint_powerplant.hdr");
    }

    void loadMaterials() override
    {
        wood = std::make_shared<CookTorranceMaterial>(woodAlbedo, woodRough, dielectricMetal);
        wood->setNormalMap(woodNormal);

        brick = std::make_shared<CookTorranceMaterial>(brickAlbedo, brickRough, dielectricMetal);
        brick->setNormalMap(brickNormal);

        marble = std::make_shared<CookTorranceMaterial>(marbleAlbedo, marbleRough, dielectricMetal);
        marble->setNormalMap(marbleNormal);

        concrete = std::make_shared<CookTorranceMaterial>(concreteAlbedo, concreteRough, dielectricMetal);
        concrete->setNormalMap(concreteNormal);

    }

    void loadScene() override
    {
        auto scene = std::make_shared<Scene>();

        scene->addShape(
            std::make_shared<Sphere>(
                glm::vec3(-3.0f,0.0f,-4.0f),
                1.0f,
                wood
            )
        );

        scene->addShape(
            std::make_shared<Sphere>(
                glm::vec3(0.0f,0.0f,-5.0f),
                1.0f,
                brick
            )
        );

        scene->addShape(
            std::make_shared<Sphere>(
                glm::vec3(3.0f,0.0f,-6.0f),
                1.0f,
                marble
            )
        );

        scene->addShape(
            std::make_shared<Plane>(
                glm::vec3(-10.f,-1.2f,4.f),
                glm::vec3(20.f,0.f,0.f),
                glm::vec3(0.f,0.f,-25.f),
                concrete
            )
        );

        world = std::make_shared<World>(scene);

        world->addLight(std::make_shared<PointLight>(glm::vec3(0.f,5.f,3.f), Color(1.0f,0.95f,0.9f), 120.f));
        world->addLight(std::make_shared<PointLight>(glm::vec3(-3.f,5.f,3.f), Color(1.0f,0.95f,0.9f), 120.f));

        auto env = std::make_shared<HDRIEnvironment>(environmentMap);
        env->setIntensity(0.8f);

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
