#ifndef __GBUFFER_HPP__
#define __GBUFFER_HPP__

enum GBufferTexture {
    POSITION = 0,
    NORMAL = 1,
    DIFFUSE = 2,
};

class GBuffer {
    public:
        GBuffer();
        ~GBuffer();
        bool init(int w, int h);

        void bindGeometryPass();

    public:
        unsigned int mFbo;
        unsigned int mTextures[3];
        unsigned int mDepth;
};

#endif