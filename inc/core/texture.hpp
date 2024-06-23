#ifndef __TEXTURE_HPP__
#define __TEXTURE_HPP__

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <string>
#include <memory>

enum class TextureType {
    DIFFUSE,
    SPECULAR,
    AMBIENT,
    NORMAL
};

class Texture {
    public:
        Texture(TextureType type, std::string path);
        ~Texture();

        void use(unsigned int textureUnitNum) {
            glBindTextureUnit(textureUnitNum, mId);
        }

        const TextureType getType() const {
            return mType;
        }
        const unsigned int getId() const {
            return mId;
        }

    private:
        unsigned int mId;
        TextureType mType;

        int mWidth, mHeight, mNrChannels;
};

typedef std::shared_ptr<Texture> TexturePtr;

#endif