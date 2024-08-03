#ifndef __DIRECTIONAL_LIGHT_HPP__
#define __DIRECTIONAL_LIGHT_HPP__

#include "core/shader.hpp"

class DirectionalLight {
    public:
        DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse) : 
            mDir(direction), mAmbient(ambient), mDiffuse(diffuse), mSpecular(1.0f, 1.0f, 1.0f) {}
        float getVolumeRadius() {
            return 0.0f;
        }
        void bindLight(ShaderPtr &shader);

    private:
        glm::vec3 mDir;
        float _DUMMY0;
        glm::vec3 mAmbient;
        float _DUMMY1;
        glm::vec3 mDiffuse;
        float _DUMMY2;
        glm::vec3 mSpecular;
        float _DUMMY3;

};

typedef std::shared_ptr<DirectionalLight> DirectionalLightPtr;
#endif