#include "buffers/transparency_FBO.hpp"

#include "glm/vec4.hpp"
#include "glm/gtc/type_ptr.hpp"

bool TransparencyFBO::init(size_t width, size_t height) {

    glCreateFramebuffers(1, &mFboID);

    glCreateTextures(GL_TEXTURE_2D, 2, &mTextures[0]);

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

void TransparencyFBO::bindForWriting() {    
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);

    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunci(ACCUM_TEXTURE, GL_ONE, GL_ONE);
    glBlendEquationi(ACCUM_TEXTURE, GL_FUNC_ADD);

    glBlendFunci(REVEAL_TEXTURE, GL_DST_COLOR, GL_ZERO);
    glBlendEquationi(REVEAL_TEXTURE, GL_FUNC_ADD);

    glBindFramebuffer(GL_FRAMEBUFFER, mFboID);
    glDrawBuffers(2, &mTextures[0]);
    GLfloat clearColor[4] = {0.0, 0.0, 0.0, 0.0};
    GLfloat clearAlpha = 1.0;
    glClearBufferfv(GL_COLOR, mTextures[ACCUM_TEXTURE], clearColor);
    glClearBufferfv(GL_COLOR, mTextures[REVEAL_TEXTURE], &clearAlpha);
}