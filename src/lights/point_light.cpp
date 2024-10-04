#include "lights/point_light.hpp"

void PointLight::bindLight(ShaderPtr &shader) {
    shader->setVec3("pointLight.position", mPos);
    shader->setVec3("pointLight.ambient", mAmbient);
    shader->setVec3("pointLight.diffuse", mDiffuse);
    shader->setVec3("pointLight.specular", mSpecular);

    shader->setFloat("pointLight.constant", mConstant);
    shader->setFloat("pointLight.linear", mLinear);
    shader->setFloat("pointLight.quadratic", mQuadratic);
    shader->setFloat("pointLight.isShadowCaster", (float)mIsShadowCaster);

    shader->setFloat("gFarPlane", 500.0f);
    mShadowMapFBO->bindForReading(5);
}

void PointLight::bindForShadowPass(ShaderPtr& shader) {
    glViewport(0, 0, mShadowMapFBO->getWidth(), mShadowMapFBO->getHeight());
    float aspect = (float)mShadowMapFBO->getWidth()/(float)mShadowMapFBO->getHeight();
    float near = 1.0f;
    float far = 500.0f;
    glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far);
    std::vector<glm::mat4> shadowTransforms;
    shadowTransforms.push_back(shadowProj * 
                    glm::lookAt(this->mPos, this->mPos + glm::vec3( 1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)));
    shadowTransforms.push_back(shadowProj * 
                    glm::lookAt(this->mPos, this->mPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)));
    shadowTransforms.push_back(shadowProj * 
                    glm::lookAt(this->mPos, this->mPos + glm::vec3( 0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
    shadowTransforms.push_back(shadowProj * 
                    glm::lookAt(this->mPos, this->mPos + glm::vec3( 0.0,-1.0, 0.0), glm::vec3(0.0, 0.0,-1.0)));
    shadowTransforms.push_back(shadowProj * 
                    glm::lookAt(this->mPos, this->mPos + glm::vec3( 0.0, 0.0, 1.0), glm::vec3(0.0,-1.0, 0.0)));
    shadowTransforms.push_back(shadowProj * 
                    glm::lookAt(this->mPos, this->mPos + glm::vec3( 0.0, 0.0,-1.0), glm::vec3(0.0,-1.0, 0.0)));

    shader->use();
    shader->setFloat("gFarPlane", far);
    shader->setVec3("gLightPos", mPos);
    for(int i = 0; i < 6; ++i) {
        shader->setMat4("gShadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
    }
    mShadowMapFBO->bindForWriting();
}