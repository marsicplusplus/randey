#include "buffers/gbuffer.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <iostream>

GBuffer::GBuffer() {
    mFbo = 0;
    mDepth = 0;
    for(int i = 0; i < 3; ++i) {
        mTextures[i] = 0;
    }
}

bool GBuffer::init(int w, int h) {
    glCreateFramebuffers(1, &mFbo);

    glCreateTextures(GL_TEXTURE_2D, GBUFFERTEXTURE_COUNT, mTextures);

    glTextureStorage2D(mTextures[GBufferTexture::POSITION], 1, GL_RGBA16F, w, h);
    glTextureParameteri(mTextures[GBufferTexture::POSITION], GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(mTextures[GBufferTexture::POSITION], GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glNamedFramebufferTexture(mFbo, GL_COLOR_ATTACHMENT0 + GBufferTexture::POSITION, mTextures[GBufferTexture::POSITION], 0);

    glTextureStorage2D(mTextures[GBufferTexture::NORMAL], 1, GL_RGBA16F, w, h);
    glTextureParameteri(mTextures[GBufferTexture::NORMAL], GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(mTextures[GBufferTexture::NORMAL], GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glNamedFramebufferTexture(mFbo, GL_COLOR_ATTACHMENT0 + GBufferTexture::NORMAL,  mTextures[GBufferTexture::NORMAL], 0);

    glTextureStorage2D(mTextures[GBufferTexture::DIFFUSE], 1, GL_RGBA8, w, h);
    glTextureParameteri(mTextures[GBufferTexture::DIFFUSE], GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(mTextures[GBufferTexture::DIFFUSE], GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glNamedFramebufferTexture(mFbo, GL_COLOR_ATTACHMENT0 + GBufferTexture::DIFFUSE, mTextures[GBufferTexture::DIFFUSE], 0);

    glCreateTextures(GL_TEXTURE_2D, 1, &mDepth);
    glTextureStorage2D(mDepth, 1, GL_DEPTH_COMPONENT32F, w, h);
    glNamedFramebufferTexture(mFbo, GL_DEPTH_ATTACHMENT, mDepth, 0);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

 return true;
}

GBuffer::~GBuffer() {
    glDeleteFramebuffers(1, &mFbo);
    glDeleteFramebuffers(1, &mDepth);
    glDeleteTextures(GBUFFERTEXTURE_COUNT, mTextures);
}

void GBuffer::bindGeometryPass() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFbo);
    unsigned int attachments[GBUFFERTEXTURE_COUNT] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(GBUFFERTEXTURE_COUNT, attachments);
}

void GBuffer::bindLightPass() {
    // glBindFramebuffer(GL_READ_FRAMEBUFFER, mFbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    for(unsigned int i = 0; i < GBUFFERTEXTURE_COUNT; i++) {
        glBindTextureUnit(i, mTextures[i]);
    }
}