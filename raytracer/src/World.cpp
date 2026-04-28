#include "World.h"

World::World(std::shared_ptr<Scene> scene)
: scene(scene) {

}

const std::shared_ptr<Scene>& World::getScene() const {
    return scene;
}

const std::vector<std::shared_ptr<Light>>& World::getLights() const {
    return lights;
}


void World::addLight(std::shared_ptr<Light> l) {
    lights.push_back(l);
}