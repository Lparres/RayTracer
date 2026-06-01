#pragma once
#include "glm/geometric.hpp"

// Clase auxiliar para simplificar la transformación de world space 
// a local space en el cálculo de la VNDF

// OrthoNormal Basis (Hietz asume que la normal está en +Z para VNDF)
class ONB {
public:
    glm::vec3 T;
    glm::vec3 B;
    glm::vec3 N;

    ONB(const glm::vec3& n) {
        N = glm::normalize(n);

        // threshold de 2.5° para controlar que N no sea casi paralelo a (0,0,1)
        // y evitar error de precisión por obtener un T demasiado pequeño
        if (std::abs(n.z) < 0.999f)  
            T = glm::normalize(glm::cross(glm::vec3(0,0,1), N));
        else
            T = glm::normalize(glm::cross(glm::vec3(0,1,0), N));

        B = glm::cross(n, T);
    }

    glm::vec3 worldToLocal(const glm::vec3& w) const {
        return glm::vec3(
            glm::dot(w,T),
            glm::dot(w,B),
            glm::dot(w,N)
        );
    }
    glm::vec3 localToWorld(const glm::vec3& w) const {
        return w.x * T + w.y * B + w.z * N;
    }
};