#ifndef __GBUFFER_HPP__
#define __GBUFFER_HPP__

enum GBufferTexture {
    POSITION = 0,
    NORMAL = 1,
    DIFFUSE = 2,
    GBUFFERTEXTURE_COUNT = 3,
};

class GBuffer {
    public:
        GBuffer();
        ~GBuffer();
        bool init(int w, int h);

        void bindGeometryPass();
        void bindLightPass();

    public:
        unsigned int mFbo;
        unsigned int mTextures[GBUFFERTEXTURE_COUNT];
        unsigned int mDepth;

    private:
        int mW, mH;
};

#endif