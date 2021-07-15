#include <string>
#include "SpotLight.h"

using namespace render::light;

SpotLight::SpotLight(glm::vec3 pos, glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float innerCone, float outerCone, light::Attenuation::Distance distance):
    m_position(pos),
    m_direction(dir),
    m_ambient(amb),
    m_diffuse(diff),
    m_specular(spec),
    m_innerConeCos(glm::cos(glm::radians(innerCone))),
    m_outerConeCos(glm::cos(glm::radians(outerCone))),
    m_attenuation(light::Attenuation::FromDistance(distance))
{}

void SpotLight::Set(Shader& shader, int index) {
    const std::string base = "spotLights[" + std::to_string(index) + "].";
    
    shader.setValue((base + "position").c_str(), m_position);
    shader.setValue((base + "direction").c_str(), m_direction);
    shader.setValue((base + "ambient").c_str(), m_ambient);
    shader.setValue((base + "diffuse").c_str(), m_diffuse);
    shader.setValue((base + "specular").c_str(), m_specular);
    shader.setValue((base + "constant").c_str(), m_attenuation.constant);
    shader.setValue((base + "linear").c_str(), m_attenuation.linear);
    shader.setValue((base + "quadratic").c_str(), m_attenuation.quadratic);
    shader.setValue((base + "innerCone").c_str(), m_innerConeCos);
    shader.setValue((base + "outerCone").c_str(), m_outerConeCos);
}

void SpotLight::SetDirection(glm::vec3 direction) {
    m_direction = direction;
}

void SpotLight::setPosition(glm::vec3 position) {
    m_position = position;
}
