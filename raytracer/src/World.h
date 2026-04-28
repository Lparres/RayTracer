#pragma once

#include <memory>
#include "Scene.h"
#include "Light.h"

// Agrupa la escena y las luces en un solo objeto para facilitar su manejo
// El World es el contexto global de la escena, que contiene toda la información necesaria para render
class World {
public:
    World(std::shared_ptr<Scene> scene);
    ~World() = default;

    const std::shared_ptr<Scene>& getScene() const;
    const std::vector<std::shared_ptr<Light>>& getLights() const;
    
    void addLight(std::shared_ptr<Light> l);
    
private:

    std::shared_ptr<Scene> scene;
    std::vector<std::shared_ptr<Light>> lights;
};