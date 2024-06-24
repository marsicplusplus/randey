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
                    mTextures[TextureType::SPECULAR] = specularTex;
                    mTextures[TextureType::AMBIENT] = ambientTex;
                    mTextures[TextureType::DIFFUSE] = diffuseTex;
            }

        void bindMaterial(Shader &shader);
        void setTexture(TextureType tType, std::shared_ptr<Texture> t) {
            mTextures[tType] = t;
        }

        glm::vec3 mDiffuseColor;
    private:
        glm::vec3 mSpecularColor;
        glm::vec3 mAmbientColor;
        std::unordered_map<TextureType, std::shared_ptr<Texture>> mTextures;
};

typedef std::shared_ptr<Material> MaterialPtr;

#endif