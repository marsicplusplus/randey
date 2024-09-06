#include "core/cubemap_texture.hpp"
#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "core/utils.hpp"

CubemapTexture::CubemapTexture(
            const std::string& PosXFilename,
            const std::string& NegXFilename,
            const std::string& PosYFilename,
            const std::string& NegYFilename,
            const std::string& PosZFilename,
            const std::string& NegZFilename) {  
        mFileNames[0] = PosXFilename;
        mFileNames[1] = NegXFilename;
        mFileNames[2] = PosYFilename;
        mFileNames[3] = NegYFilename;
        mFileNames[4] = PosZFilename;
        mFileNames[5] = NegZFilename;
}

bool CubemapTexture::load() {
    glGenTextures(1, &mTextureObj);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureObj);

    for(int i = 0; i < 6; ++i){
        int w, h, nrChannels;
        unsigned char* data = Images::loadImage(mFileNames[i], w, h, nrChannels);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        Images::freeData(data);
    }
    glTextureParameteri(mTextureObj, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(mTextureObj, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(mTextureObj, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(mTextureObj, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(mTextureObj, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    return true;
}

void CubemapTexture::bind(unsigned int textureUnit) {
    glBindTextureUnit(textureUnit, mTextureObj);
}

CubemapTexture::~CubemapTexture() {
    glDeleteTextures(1, &mTextureObj);
    mTextureObj = 0;
}