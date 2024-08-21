#ifndef __DIRECTIONAL_LIGHT_HPP__
#define __DIRECTIONAL_LIGHT_HPP__

#include "core/shader.hpp"
#include "buffers/shadow_map_FBO.hpp"

class DirectionalLight {
    public:
        DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, ShadowMapFBOPtr shadowmap) : 
            mDir(glm::normalize(direction)), mAmbient(ambient), mDiffuse(diffuse), 
                mSpecular(1.0f, 1.0f, 1.0f), mShadowMapFBO(std::move(shadowmap)) {}
        float getVolumeRadius() {
            return 0.0f;
        }
        void bindLight(ShaderPtr &shader);
        void bindForShadowPass(ShaderPtr shader);

    private:
        ShadowMapFBOPtr mShadowMapFBO;

        glm::vec3 mDir;
        glm::vec3 mAmbient;
        glm::vec3 mDiffuse;
        glm::vec3 mSpecular;

};

typedef std::shared_ptr<DirectionalLight> DirectionalLightPtr;
#endif