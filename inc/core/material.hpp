#ifndef __MATERIAL_HPP__
#define __MATERIAL_HPP__

#include "glm/vec3.hpp"

#include "core/shader.hpp"
#include "core/texture.hpp"

#include <memory>

class Material {
    public:
        Material(
            glm::vec3 diffuse, 
            TexturePtr diffuseTex,
            glm::vec3 specular, 
            TexturePtr specularTex,
            glm::vec3 ambient,
            TexturePtr ambientTex) :
            mDiffuseColor(diffuse), mSpecularColor(specular), mAmbientColor(ambient) {
                if(specularTex) {
                    mTextures[TextureType::SPECULAR] = specularTex;
                }
                if(ambientTex) {
                    mTextures[TextureType::AMBIENT] = ambientTex;
                }
                if(diffuseTex) {
                    mTextures[TextureType::DIFFUSE] = diffuseTex;
                }
            }

        void bindMaterial(Shader &shader);
        void setTexture(TextureType tType, std::shared_ptr<Texture> t) {
            mTextures[tType] = t;
        }

    private:
        glm::vec3 mDiffuseColor;
        glm::vec3 mSpecularColor;
        glm::vec3 mAmbientColor;
        std::unordered_map<TextureType, std::shared_ptr<Texture>> mTextures;
};

#endif