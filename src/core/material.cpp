#include "core/material.hpp"

void Material::bindMaterial(ShaderPtr &shader) {
    shader->setFloat("material.shininess", 32.0f);
    shader->setInt("material.diffuseTexture", 0);
    shader->setInt("material.specularTexture", 1);
    shader->setInt("material.ambientTexture", 2);
    shader->setInt("material.alphaTexture", 3);
    if(mTextures.find(TextureType::DIFFUSE) != mTextures.end()) {
        glBindTextureUnit(0, mTextures[TextureType::DIFFUSE]);
    }
    if(mTextures.find(TextureType::SPECULAR) != mTextures.end()) {
        glBindTextureUnit(1, mTextures[TextureType::SPECULAR]);
    }
    if(mTextures.find(TextureType::AMBIENT) != mTextures.end()) {
        glBindTextureUnit(2, mTextures[TextureType::AMBIENT]);
    }
    if(mTextures.find(TextureType::ALPHA) != mTextures.end()) {
        glBindTextureUnit(3, mTextures[TextureType::ALPHA]);
    }
}