#include "core/material.hpp"

void Material::bindMaterial(ShaderPtr &shader) {
    shader->setFloat("material.shininess", 32.0f);
    shader->setInt("material.diffuseTexture", 0);
    shader->setInt("material.specularTexture", 1);
    if(mTextures[TextureType::DIFFUSE]) {
        glBindTextureUnit(0, mTextures[TextureType::DIFFUSE]->getId());
    }
    if(mTextures[TextureType::SPECULAR]) {
        glBindTextureUnit(1, mTextures[TextureType::SPECULAR]->getId());
    }
}