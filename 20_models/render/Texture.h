#pragma once

#include <string>
#include <assimp/scene.h>

namespace render {

struct Texture {
    enum Type { diffuse, specular };
    
    unsigned int id;
    Type type;
    std::string path;
    
    static aiTextureType GetAssimpType(Texture::Type type);
};

}
