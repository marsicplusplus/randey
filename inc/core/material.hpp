#ifndef __MATERIAL_HPP__
#define __MATERIAL_HPP__

#include "glm/vec3.hpp"

#include "core/shader.hpp"
#include "core/texture.hpp"

#include <memory>

class Material {
    public:
        Material(TexturePtr diffuseTex, TexturePtr specularTex) {
                    mTextures[TextureType::SPECULAR] = specularTex;
                    mTextures[TextureType::DIFFUSE] = diffuseTex;
            }

        void bindMaterial(ShaderPtr &shader);
        void setTexture(TextureType tType, std::shared_ptr<Texture> t) {
            mTextures[tType] = t;
        }

    private:
        std::unordered_map<TextureType, std::shared_ptr<Texture>> mTextures;
};

typedef std::shared_ptr<Material> MaterialPtr;

#endif