#include "lights/point_light.hpp"

void PointLight::bindLight(ShaderPtr &shader) {
    shader->setVec3("pointLight.position", mPos);
    shader->setVec3("pointLight.ambient", mAmbient);
    shader->setVec3("pointLight.diffuse", mDiffuse);
    shader->setVec3("pointLight.specular", mSpecular);

    shader->setFloat("pointLight.constant", mConstant);
    shader->setFloat("pointLight.linear", mLinear);
    shader->setFloat("pointLight.quadratic", mQuadratic);
}