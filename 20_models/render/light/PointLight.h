#pragma once

#include <glm/glm.hpp>
#include "Attenuation.h"
#include "Shader.h"

namespace render::light {

class PointLight {
public:
    glm::vec3 m_position;
    
    PointLight(glm::vec3 pos, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, Attenuation::Distance distance);
    void Set(Shader& shader, int index);
private:
    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;
    Attenuation m_attenuation;
};

}
