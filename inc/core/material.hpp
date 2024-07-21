#ifndef __MATERIAL_HPP__
#define __MATERIAL_HPP__

#include "glm/vec3.hpp"

#include "core/shader.hpp"
#include "core/texture.hpp"

#include <memory>

class Material {
    public:
        Material(unsigned int diffuseTex, unsigned int specularTex, unsigned int ambientTex) {
                    mTextures[TextureType::SPECULAR] = specularTex;
                    mTextures[TextureType::DIFFUSE] = diffuseTex;
                    mTextures[TextureType::AMBIENT] = ambientTex;
            }

        void bindMaterial(ShaderPtr &shader);
        void setTexture(TextureType tType, unsigned int t) {
            mTextures[tType] = t;
        }

    private:
        std::unordered_map<TextureType, unsigned int> mTextures;
};

typedef std::shared_ptr<Material> MaterialPtr;

#endif