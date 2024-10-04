#include "lights/directional_light.hpp"

void DirectionalLight::bindLight(ShaderPtr &shader) {
    shader->setVec3("dirLight.direction", mDir);
    shader->setVec3("dirLight.ambient", mAmbient);
    shader->setVec3("dirLight.diffuse", mDiffuse);
    shader->setVec3("dirLight.specular", mSpecular);
    shader->setFloat("isShadowCaster", mIsShadowCaster);
    glm::mat4 lightView = glm::lookAt(
        glm::vec3(0.0, 0.0, 0.0),
        this->mDir,
        glm::vec3(0.0, 1.0, 0.0)
    );
    shader->setMat4("lightView", lightView);
    glm::mat4 lightProj = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, -100.0f, 100.0f);
    shader->setMat4("lightProj", lightProj);

    if(mIsShadowCaster) {
        mShadowMapFBO->bindForReading(5);
    }
}

void DirectionalLight::bindForShadowPass(ShaderPtr shader) {
    mShadowMapFBO->bindForWriting();
    glm::mat4 lightProjection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, -100.0f, 100.0f);
    glm::mat4 lightView = glm::lookAt(
        glm::vec3(0.0, 0.0, 0.0),
        this->mDir,
        glm::vec3(0.0, 1.0, 0.0)
    );
    shader->setMat4("view", lightView);
    shader->setMat4("projection", lightProjection);
}