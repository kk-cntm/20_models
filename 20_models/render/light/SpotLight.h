#pragma once

#include <glm/glm.hpp>
#include "Attenuation.h"
#include "../Shader.h"

namespace render::light {

class SpotLight {
public:
    SpotLight(glm::vec3 pos, glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float innerCone, float outerCone, light::Attenuation::Distance distance);
    void Set(Shader& shader, int index);
    void SetDirection(glm::vec3 direction);
    void setPosition(glm::vec3 position);
private:
    glm::vec3 m_position;
    glm::vec3 m_direction;
    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;
    float m_innerConeCos;
    float m_outerConeCos;
    light::Attenuation m_attenuation;
};

}
