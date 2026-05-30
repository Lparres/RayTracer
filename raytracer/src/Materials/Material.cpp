#include "Material.h"
#include "HitInfo.h"
#include "glm/geometric.hpp"
#include "glm/matrix.hpp"

float Material::ambientOcclusion(UV) const
{
    return 1.0f;
}

glm::vec3 Material::shadingNormal(const HitInfo& hit) const
{
    return hit.normal;
}

glm::vec3 Material::perturbNormal(const HitInfo& hit, const Texture& normalMap)
{
    // La textura almacena normales en [0,1] — remapear a [-1,1]
    const Color sample = normalMap.sample(hit.uv);
    const glm::vec3 tangentNormal = glm::normalize(glm::vec3(
        sample.r * 2.f - 1.f,
        sample.g * 2.f - 1.f,
        sample.b * 2.f - 1.f
    ));

    // TBN: matriz de cambio de base de tangent space a world space
    // Columnas: tangente (eje U), bitangente (eje V), normal (eje Z)
    const glm::mat3 TBN(hit.tangent, hit.bitangent, hit.normal);

    return glm::normalize(TBN * tangentNormal);
}
