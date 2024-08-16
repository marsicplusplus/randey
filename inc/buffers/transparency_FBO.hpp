#ifndef __TRANSPARENCY_FBO_HPP__
#define __TRANSPARENCY_FBO_HPP__

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <memory>

enum {
    ACCUM_TEXTURE = 0,
    REVEAL_TEXTURE = 1,
};

class TransparencyFBO {
    public:
        TransparencyFBO() {
            mFboID = 0;
            mTextures[0] = 0;
            mTextures[1] = 0;
        };

        ~TransparencyFBO(); 

        bool init(size_t width, size_t height);
        void blitDepthBuffer(const unsigned int srcFBO, size_t w, size_t h);
        void bindForGather();
        void bindForComposite();
        void bindForReading(unsigned int textureUnit);

        private:
            unsigned int mFboID;
            unsigned int mTextures[2];
};

#endif