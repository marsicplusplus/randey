#ifndef __MATERIAL_HPP__
#define __MATERIAL_HPP__

#include "glm/vec3.hpp"

#include "core/shader.hpp"
#include "core/texture.hpp"

#include <memory>

class Material {
    public:
        Material(unsigned int diffuseTex, unsigned int specularTex, unsigned int ambientTex, unsigned int alphaTex, unsigned int normalTex) {
                    mTextures[TextureType::SPECULAR] = specularTex;
                    mTextures[TextureType::DIFFUSE] = diffuseTex;
                    mTextures[TextureType::AMBIENT] = ambientTex;
                    mTextures[TextureType::ALPHA] = alphaTex;
                    mTextures[TextureType::NORMAL] = normalTex;
            }

        void bindMaterial(ShaderPtr &shader);
        void setTexture(TextureType tType, unsigned int t) {
            mTextures[tType] = t;
        }
        bool hasAlpha() const {
            return (mTextures.at(TextureType::ALPHA) != 0);
        }

    private:
        std::unordered_map<TextureType, unsigned int> mTextures;
};

typedef std::shared_ptr<Material> MaterialPtr;

#endif