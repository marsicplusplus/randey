#include "managers/resource_manager.hpp"

#include "stb_image.h"

ResourceManager* ResourceManager::mInstance = nullptr;


unsigned int ResourceManager::setTexture(const std::string &path, bool flip) {

    const auto val = mTextures.find(path);
    if(val != mTextures.end()) {
        return val->second;
    }
    
    unsigned int textId;
    glCreateTextures(GL_TEXTURE_2D, 1, &textId);

    glTextureParameteri(textId, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(textId, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(textId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(textId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(flip);
    int w,h,nrChannels;
    unsigned char* data = stbi_load(path.c_str(), &w, &h, &nrChannels, 0);
    if(data == nullptr){
        return 0;
    }
    GLenum format = 0;
    GLenum internalFormat = 0;
    if(nrChannels == 1) {
        format = GL_RED;
        internalFormat = GL_R8;
    } else if (nrChannels == 2) {
        format = GL_RG;
        internalFormat = GL_RG16F;
    } else if (nrChannels == 3) {
        format = GL_RGB;
        internalFormat = GL_RGB8;
    } else if (nrChannels == 4) {
        format = GL_RGBA;
        internalFormat = GL_RGBA8;
    }

    glTextureStorage2D(textId, 1, internalFormat, w, h);
    glTextureSubImage2D(textId, 0, 0, 0, w, h, format, GL_UNSIGNED_BYTE, data);
    glGenerateTextureMipmap(textId);
    stbi_image_free(data);

    return mTextures.try_emplace(path, textId).first->second;
}



unsigned int ResourceManager::getTexture(const std::string &path) {
    const auto val = mTextures.find(path); 
    if(val != mTextures.end()) {
        return val->second;
    }
    return 0;
}