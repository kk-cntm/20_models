#pragma once

#include <glm/glm.hpp>
#include <array>
#include "../Shader.h"
#include "../Vertex.h"
#include "../IDrawable.h"
#include "../Material.h"

namespace render::shape {

class Cuboid: public IDrawable {
public:
    Cuboid(glm::vec3 position, Material mat);
    Cuboid(glm::vec3 position, Material mat, glm::vec3 size);
    void Draw(Shader& shader) const;
private:
    static const std::array<Vertex, 288> s_vertices;
    static int s_instances;
    static unsigned int s_vao;
    static unsigned int s_vbo;
    
    static void Setup();
    
    glm::vec3 m_size;
    
    glm::vec3 m_position;
    Material m_material;
};

}
