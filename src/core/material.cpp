#include "core/material.hpp"

void Material::bindMaterial(Shader &shader) {
    shader.setVec3("material.ambient", mAmbientColor);
    shader.setVec3("material.diffuse", mDiffuseColor);
    shader.setVec3("material.specular", mSpecularColor);
    shader.setFloat("material.shininess", 32.0f);
}