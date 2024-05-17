#ifndef __TEXTURE_HPP__
#define __TEXTURE_HPP__


enum class TextureType {
    DIFFUSE,
    SPECULAR
};

class Texture {
    public:
        Texture();
        ~Texture();
        const TextureType getType() const {
            return mType;
        }
        const unsigned int getId() const {
            return mId;
        }

    private:
        unsigned int mId;
        TextureType mType;
};

#endif