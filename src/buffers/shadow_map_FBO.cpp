#include "buffers/shadow_map_FBO.hpp"

bool ShadowMapFBO::init(unsigned int w, unsigned int h, unsigned int type) {

    mType = type;

    glCreateFramebuffers(1, &mFboID);

    glCreateTextures(mType, 1, &mShadowMapID);

    glTextureParameterf(mShadowMapID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameterf(mShadowMapID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(mShadowMapID, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTextureParameteri(mShadowMapID, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTextureParameterf(mShadowMapID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameterf(mShadowMapID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    if(type == GL_TEXTURE_CUBE_MAP) {
        for (int face = 0; face < 6; ++face)
        {
            // face:
            // 0 = positive x face
            // 1 = negative x face
            // 2 = positive y face
            // 3 = negative y face
            // 4 = positive z face
            // 5 = negative z face
            glTextureSubImage3D(
                mShadowMapID,
                0, // only 1 level in example
                0,
                0,
                face, // the offset to desired cubemap face, which offset goes to which face above
                w,
                h,
                1, // depth how many faces to set, if this was 3 we'd set 3 cubemap faces at once
                GL_DEPTH_COMPONENT,
                GL_FLOAT,
                NULL);
        }
    } else {
        glTextureStorage2D(mShadowMapID, 1, GL_DEPTH_COMPONENT32, w, h);
    }

    glNamedFramebufferTexture(mFboID, GL_DEPTH_ATTACHMENT, mShadowMapID, 0);

    // unsigned int status = glCheckNamedFramebufferStatus(&mFboID, GL_FRAMEBUFFER);
    // if(status != GL_FRAMEBUFFER_COMPLETE) {
    //     return false;
    // }
    return true;
}

void ShadowMapFBO::bindForWriting(){
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFboID);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
}

void ShadowMapFBO::bindForReading(unsigned int textureUnit){
    glBindTextureUnit(textureUnit, mShadowMapID);
}

