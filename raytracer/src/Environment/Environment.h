#pragma once
#include "Color.h"

class Environment {
public:
    virtual ~Environment() = default;

    Color sampleScaled(const glm::vec3& rayDir) const {
        return sample(rayDir) * _intensity;
    }

    void  setIntensity(float intensity) { _intensity = intensity; }
    float getIntensity() const          { return _intensity; }

protected:
    virtual Color sample(const glm::vec3& rayDir) const = 0;

private:
    float _intensity = 1.0f;
};
