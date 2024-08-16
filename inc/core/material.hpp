#ifndef __MATERIAL_HPP__
#define __MATERIAL_HPP__

#include "glm/vec3.hpp"

#include "core/shader.hpp"
#include "core/texture.hpp"

#include <memory>

class Material {
    public:
        Material(unsigned int diffuseTex, unsigned int specularTex, unsigned int ambientTex, unsigned int alphaTex) {
                    mTextures[TextureType::SPECULAR] = specularTex;
                    mTextures[TextureType::DIFFUSE] = diffuseTex;
                    mTextures[TextureType::AMBIENT] = ambientTex;
                    mTextures[TextureType::ALPHA] = alphaTex;
            }

        void bindMaterial(ShaderPtr &shader);
        void setTexture(TextureType tType, unsigned int t) {
            mTextures[tType] = t;
        }
        unsigned int getTexture(TextureType tType) const {
            const auto texture = mTextures.find(tType);
            return texture == mTextures.end() ? 0 : texture->second;
        }

    private:
        std::unordered_map<TextureType, unsigned int> mTextures;
};

typedef std::shared_ptr<Material> MaterialPtr;

#endif