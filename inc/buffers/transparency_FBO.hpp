#ifndef __TRANSPARENCY_FBO_HPP__
#define __TRANSPARENCY_FBO_HPP__

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <memory>

enum {
    ACCUM_TEXTURE = 0,
    REVEAL_TEXTURE = 1
};

class TransparencyFBO {
    public:
        TransparencyFBO() {};
        ~TransparencyFBO()
        {
            glDeleteFramebuffers(1, &mFboID);
            glDeleteTextures(2, &mTextures[0]);
            mFboID = 0;
            mTextures[ACCUM_TEXTURE] = 0;
            mTextures[REVEAL_TEXTURE] = 0;
        }

        bool init(size_t width, size_t height);
        void bindForWriting();
        void bindForReading(unsigned int textureUnit);

        private:
            unsigned int mFboID;
            unsigned int mTextures[2];
};

#endif