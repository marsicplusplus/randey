#ifndef __POINT_LIGHT_HPP__
#define __POINT_LIGHT_HPP__

#include "core/shader.hpp"

class PointLight {
    public:
        PointLight(glm::vec3 pos, glm::vec3 ambient, glm::vec3 diffuse, bool isShadowCaster = false) : 
            mPos(glm::vec4(pos, 1.0)), mAmbient(glm::vec4(ambient, 0.0)), mDiffuse(glm::vec4(diffuse, 0.0)), mSpecular(1.0f, 1.0f, 1.0f, 0.0f) {}
        float getVolumeRadius() {
            float maxChannel = std::max(std::max(mAmbient.x, mAmbient.y), mAmbient.z);
            float radius = (-mAttenuationTerms.y +  std::sqrt(mAttenuationTerms.y * mAttenuationTerms.y - 4 * mAttenuationTerms.z * (mAttenuationTerms.x - (256.0 / 5.0) * maxChannel))) 
                            / (2 * mAttenuationTerms.z); 
            return radius;
        }
        void bindLight(ShaderPtr &shader);
        glm::vec3 getPosition() const {return mPos;}
        glm::vec3 getDiffuse() const {return mDiffuse; };
        void* getDataPointer() {
            return (void*)&mPos;
        }
        static size_t getDataSize() {
            return 5 * sizeof(glm::vec4);
        }

    private:
        glm::vec4 mPos;
        glm::vec4 mAmbient;
        glm::vec4 mDiffuse;
        glm::vec4 mSpecular;
        glm::vec4 mAttenuationTerms; // [constant, linear, quadratic, isShadowCaster]
};

typedef std::shared_ptr<PointLight> PointLightPtr;
#endif