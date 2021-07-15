#include <string>
#include "PointLight.h"

using namespace render::light;

PointLight::PointLight(glm::vec3 pos, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, Attenuation::Distance distance):
    m_position(pos),
    m_ambient(amb),
    m_diffuse(diff),
    m_specular(spec),
    m_attenuation(Attenuation::FromDistance(distance))
{}

void PointLight::Set(Shader& shader, int index) {
    const std::string base = "pointLights[" + std::to_string(index) + "].";
    
    shader.setValue((base + "position").c_str(), m_position);
    shader.setValue((base + "ambient").c_str(), m_ambient);
    shader.setValue((base + "diffuse").c_str(), m_diffuse);
    shader.setValue((base + "specular").c_str(), m_specular);
    shader.setValue((base + "constant").c_str(), m_attenuation.constant);
    shader.setValue((base + "linear").c_str(), m_attenuation.linear);
    shader.setValue((base + "quadratic").c_str(), m_attenuation.quadratic);
}
