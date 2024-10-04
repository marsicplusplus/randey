#ifndef __POINT_LIGHT_HPP__
#define __POINT_LIGHT_HPP__

#include "core/shader.hpp"
#include "buffers/shadow_map_FBO.hpp"

#include <iostream>

class PointLight {
    public:
        PointLight(glm::vec3 pos, glm::vec3 ambient, glm::vec3 diffuse, ShadowMapFBOPtr shadowMap, bool isShadowCaster = false) : 
            mPos(pos), mAmbient(ambient), mDiffuse(diffuse), mSpecular(1.0f, 1.0f, 1.0f), 
            mShadowMapFBO(std::move(shadowMap)), mIsShadowCaster(isShadowCaster) {}
        float getVolumeRadius() {
            float maxChannel = std::max(std::max(mAmbient.x, mAmbient.y), mAmbient.z);
            float radius = (-mLinear +  std::sqrt(mLinear * mLinear - 4 * mQuadratic * (mConstant - (256.0 / 5.0) * maxChannel))) 
                            / (2 * mQuadratic); 
            return radius;
        }
        void bindLight(ShaderPtr &shader);
        void bindForShadowPass(ShaderPtr &shader);
        glm::vec3 getPosition() const {return mPos;}
        glm::vec3 getDiffuse() const {return mDiffuse; };
        bool isShadowCaster() const {return mIsShadowCaster; }

    private:
        ShadowMapFBOPtr mShadowMapFBO;

        glm::vec3 mPos;
        glm::vec3 mAmbient;
        glm::vec3 mDiffuse;
        glm::vec3 mSpecular;

        float mConstant = 1.0;
        float mLinear = 0.045;
        float mQuadratic = 0.0075;
        bool mIsShadowCaster = false;
};

typedef std::shared_ptr<PointLight> PointLightPtr;
#endif