#include "World.h"
#include "Scene.h"
#include "Light.h"
#include "Environment.h"

World::World(std::shared_ptr<Scene> scene)
: scene(scene)
{}

const Scene& World::getScene() const {
    return *scene;
}

const std::vector<std::shared_ptr<Light>>& World::getLights() const {
    return lights;
}

void World::addLight(std::shared_ptr<Light> light) {
    lights.push_back(light);
}

void World::setEnvironment(std::shared_ptr<Environment> enviro) {
    environment = enviro;
}

Color World::sampleEnvironment(const glm::vec3& rayDir) const {
    if(environment == nullptr) return Color(0.f);
    return environment->sampleScaled(rayDir);
}
