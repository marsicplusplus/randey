#ifndef __SHADOW_MAP_HPP__
#define __SHADOW_MAP_HPP__

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <memory>

class ShadowMapFBO {
    public:
        ShadowMapFBO() {};
        ~ShadowMapFBO() {
            glDeleteFramebuffers(1, &mFboID);
            glDeleteTextures(1, &mShadowMapID);
            mFboID = 0;
            mShadowMapID = 0;
        }

        bool init(unsigned int width, unsigned int height, unsigned int type = GL_TEXTURE_2D);
        void bindForWriting();
        void bindForReading(unsigned int textureUnit);

        private:
            unsigned int mFboID;
            unsigned int mShadowMapID;
            unsigned int mType;

};

typedef std::unique_ptr<ShadowMapFBO> ShadowMapFBOPtr;

#endif