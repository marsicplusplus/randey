#include "lights/directional_light.hpp"

void DirectionalLight::bindLight(ShaderPtr &shader) {
    shader->setVec3("dirLight.direction", mDir);
    shader->setVec3("dirLight.ambient", mAmbient);
    shader->setVec3("dirLight.diffuse", mDiffuse);
    shader->setVec3("dirLight.specular", mSpecular);
}