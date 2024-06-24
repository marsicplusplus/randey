#include "core/material.hpp"

void Material::bindMaterial(Shader &shader) {
    // shader.setVec3("material.ambient", mAmbientColor);
    // shader.setVec3("material.diffuse", mDiffuseColor);
    // shader.setVec3("material.specular", mSpecularColor);
    shader.setFloat("material.shininess", 32.0f);
    shader.setInt("material.diffuseTexture", 0);
    shader.setInt("material.ambientTexture", 1);
    shader.setInt("material.specularTexture", 2);
    if(mTextures[TextureType::DIFFUSE]) {
        glBindTextureUnit(0, mTextures[TextureType::DIFFUSE]->getId());
    }
    if(mTextures[TextureType::AMBIENT]) {
        glBindTextureUnit(1, mTextures[TextureType::AMBIENT]->getId());
    }
    if(mTextures[TextureType::SPECULAR]) {
        glBindTextureUnit(2, mTextures[TextureType::SPECULAR]->getId());
    }
}