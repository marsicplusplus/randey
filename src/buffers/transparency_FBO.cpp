#include "buffers/transparency_FBO.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"


#include "glm/vec4.hpp"
#include "glm/gtc/type_ptr.hpp"

bool TransparencyFBO::init(size_t width, size_t height) {
    glCreateFramebuffers(1, &mFboID);

    glCreateTextures(GL_TEXTURE_2D, 3, &mTextures[0]);

    glTextureStorage2D(mTextures[ACCUM_TEXTURE], 1, GL_RGBA16F, width, height);
    glTextureParameteri(mTextures[ACCUM_TEXTURE], GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(mTextures[ACCUM_TEXTURE], GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glNamedFramebufferTexture(mFboID, GL_COLOR_ATTACHMENT0 + ACCUM_TEXTURE, mTextures[ACCUM_TEXTURE], 0);

    glTextureStorage2D(mTextures[REVEAL_TEXTURE], 1, GL_R16F, width, height);
    glTextureParameteri(mTextures[REVEAL_TEXTURE], GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(mTextures[REVEAL_TEXTURE], GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glNamedFramebufferTexture(mFboID, GL_COLOR_ATTACHMENT0 + REVEAL_TEXTURE, mTextures[REVEAL_TEXTURE], 0);

    return true;
}

void TransparencyFBO::bindForComposite() {    
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glDepthMask(GL_TRUE);
    // glBlendEquation(GL_FUNC_ADD);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);

    // glBindTextureUnit(0, mTextures[ACCUM_TEXTURE]);
    // glBindTextureUnit(1, mTextures[REVEAL_TEXTURE]);
}

void TransparencyFBO::bindForGather() {    
    glBindFramebuffer(GL_FRAMEBUFFER, mFboID);
    unsigned int attachments[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2, attachments);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    // glDepthFunc(GL_LEQUAL);

    // glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);

    GLfloat clearColor[4] = {0.0, 0.0, 0.0, 0.0};
    glClearBufferfv(GL_COLOR, ACCUM_TEXTURE, clearColor);
    glBlendFunci(ACCUM_TEXTURE, GL_ONE, GL_ONE);
    glBlendEquationi(ACCUM_TEXTURE, GL_FUNC_ADD);

    GLfloat clearAlpha = 1.0;
    glClearBufferfv(GL_COLOR, REVEAL_TEXTURE, &clearAlpha);
    glBlendFunci(REVEAL_TEXTURE, GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
    glBlendEquationi(REVEAL_TEXTURE, GL_FUNC_ADD);
}


TransparencyFBO::~TransparencyFBO() {
    glDeleteFramebuffers(1, &mFboID);
    glDeleteTextures(2, &mTextures[0]);
    mFboID = 0;
    mTextures[ACCUM_TEXTURE] = 0;
    mTextures[REVEAL_TEXTURE] = 0;
}

void TransparencyFBO::bindForReading(unsigned int textureUnit) {

}