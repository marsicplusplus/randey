#include "core/material.hpp"

void Material::bindMaterial(ShaderPtr &shader) {
    shader->setFloat("material.shininess", 32.0f);
    shader->setInt("material.diffuseTexture", 0);
    shader->setInt("material.specularTexture", 1);
    shader->setInt("material.ambientTexture", 2);
    shader->setInt("material.alphaTexture", 3);
    if(mTextures[TextureType::DIFFUSE] > 0) {
        glBindTextureUnit(0, mTextures[TextureType::DIFFUSE]);
    }
    if(mTextures[TextureType::SPECULAR] > 0) {
        glBindTextureUnit(1, mTextures[TextureType::SPECULAR]);
    }
    if(mTextures[TextureType::AMBIENT] > 0) {
        glBindTextureUnit(2, mTextures[TextureType::AMBIENT]);
    }
    if(mTextures[TextureType::ALPHA] > 0) {
        glBindTextureUnit(3, mTextures[TextureType::ALPHA]);
    }
}
