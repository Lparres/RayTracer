#pragma once

#include "DemoScene.h"
#include "DemoIncludes.h"

class Demo2 : public DemoScene
{
private:
    Film film;
    Camera cam;
    std::shared_ptr<World> world;


    // Textures
    std::shared_ptr<ConstantTexture> concreteTexture;
    std::shared_ptr<ImageTexture> environmentMap;

    // Materials
    std::vector<std::shared_ptr<Material>> sphereMaterials;
    std::shared_ptr<Material> groundMaterial;

public:
    Demo2()
    : DemoScene("Demo2")
    , film(1920,1080)
    , cam(
        {0.0f, 2.5f, 10.0f},
        {0.0f, 0.0f, -4.0f},
        {0.0f, 1.0f, 0.0f},
        film,
        35.0f,
        0.0f,
        10.0f)
    {}

    ~Demo2() = default;

    void loadTextures() override
    {
        concreteTexture = ConstantTexture::createTexture(Color(0.75f,0.75f,0.75f));

        environmentMap = ImageTexture::createTexture("belfast_sunset_puresky.hdr");
    }

    void loadMaterials() override
    {
        const Color showcaseColor(0.95f,0.65f,0.20f);

        sphereMaterials.clear();

        const float metallicValues[3] =
        {
            1.0f,
            0.5f,
            0.0f
        };

        const float roughnessValues[5] =
        {
            0.02f,
            0.15f,
            0.35f,
            0.65f,
            1.00f
        };

        for(int row=0; row<3; ++row)
        {
            for(int col=0; col<5; ++col)
            {
                sphereMaterials.push_back(
                    std::make_shared<CookTorranceMaterial>(
                        showcaseColor,
                        roughnessValues[col],
                        metallicValues[row]
                    )
                );
            }
        }

        groundMaterial =
            std::make_shared<CookTorranceMaterial>(
                concreteTexture,
                0.7f,
                0.0f
            );
    }

    void loadScene() override
    {
        auto scene = std::make_shared<Scene>();

        const float radius = 0.8f;

        const float xs[5] =
        {
            -4.f,
            -2.f,
            0.f,
            2.f,
            4.f
        };

        const float ys[3] =
        {
            2.f,
            0.f,
            -2.f
        };

        int materialIndex = 0;

        for(int row=0; row<3; ++row)
        {
            for(int col=0; col<5; ++col)
            {
                scene->addShape(
                    std::make_shared<Sphere>(
                        glm::vec3(
                            xs[col],
                            ys[row],
                            -6.f),
                        radius,
                        sphereMaterials[materialIndex++]
                    )
                );
            }
        }

        scene->addShape(
            std::make_shared<Plane>(
                glm::vec3(-8.f,-3.5f,2.f),
                glm::vec3(16.f,0.f,0.f),
                glm::vec3(0.f,0.f,-20.f),
                groundMaterial
            )
        );

        world = std::make_shared<World>(scene);

        world->addLight(std::make_shared<PointLight>(glm::vec3(0.f,8.f,4.f), WHITE, 60.f));

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
