#ifndef __CUBE_TEXTURE_HPP__
#define __CUBE_TEXTURE_HPP__

#include <string>

class CubemapTexture {
    public:
        CubemapTexture(const std::string& Directory,
            const std::string& PosXFilename,
            const std::string& NegXFilename,
            const std::string& PosYFilename,
            const std::string& NegYFilename,
            const std::string& PosZFilename,
            const std::string& NegZFilename);

        ~CubemapTexture();

        bool load();

        void bind(unsigned int TextureUnit);

private:

        std::string mFileNames[6];
        unsigned int mTextureObj;
};

#endif