#pragma once

#include <memory>

#include "Texture.h"

class PBRMaterial {
public:
    explicit PBRMaterial(
        std::shared_ptr<Texture> albedoMap, 
        std::shared_ptr<Texture> roughnessMap, 
        std::shared_ptr<Texture> metallicMap,
        std::shared_ptr<Texture> normalMap
    );
    


};
