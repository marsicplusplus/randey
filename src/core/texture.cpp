#include "core/texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
Texture::Texture(TextureType type, std::string path) : mType(type) {
    glCreateTextures(GL_TEXTURE_2D, 1, &mId);

    glTextureParameteri(mId, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(mId, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(mId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(mId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path.c_str(), &mWidth, &mHeight, &mNrChannels, 0);
    if (data)
    {
        glTextureStorage2D(mId, 1, GL_RGB8, mWidth, mHeight);
        glTextureSubImage2D(mId, 0, 0, 0, mWidth, mHeight, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateTextureMipmap(mId);
    }
    stbi_image_free(data);
} 

Texture::~Texture() {
    glDeleteTextures(1, &mId);
}