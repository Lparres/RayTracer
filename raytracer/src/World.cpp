#include "World.h"

World::World(std::shared_ptr<Scene> scene)
: scene(scene) {

}

std::shared_ptr<Scene> World::getScene() {
    return scene;
}

std::vector<std::shared_ptr<Light>> World::getLights() {
    return lights;
}


void World::addLight(std::shared_ptr<Light> l) {
    lights.push_back(l);
}