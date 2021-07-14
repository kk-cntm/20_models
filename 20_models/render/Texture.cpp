#include <iostream>
#include "Texture.h"

using namespace render;

aiTextureType Texture::GetAssimpType(Texture::Type type) {
    switch (type) {
        case Texture::diffuse: return aiTextureType_DIFFUSE;
        case Texture::specular: return aiTextureType_SPECULAR;
        default: std::cerr << "[ERROR] Texture::GetAssimpType invalid type\n";
    }
}
