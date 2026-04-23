#pragma once

#include <memory>
#include "Scene.h"
#include "Light.h"

class World {
public:
    World(std::shared_ptr<Scene> scene);
    ~World() = default;

    std::shared_ptr<Scene> getScene();
    std::vector<std::shared_ptr<Light>> getLights();
    
    void addLight(std::shared_ptr<Light> l);
    
private:

    std::shared_ptr<Scene> scene;
    std::vector<std::shared_ptr<Light>> lights;
};