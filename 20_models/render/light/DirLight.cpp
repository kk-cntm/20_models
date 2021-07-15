#include "DirLight.h"

using namespace render::light;

DirLight::DirLight(glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec):
    m_direction(dir),
    m_ambient(amb),
    m_diffuse(diff),
    m_specular(spec)
{}

void DirLight::Set(Shader& shader) {
    shader.setValue("dirLight.direction", m_direction);
    shader.setValue("dirLight.ambient", m_ambient);
    shader.setValue("dirLight.diffuse", m_diffuse);
    shader.setValue("dirLight.specular", m_specular);
}
