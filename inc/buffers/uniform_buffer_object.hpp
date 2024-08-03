#ifndef __UNIFORM_BUFFER_OBJECT_HPP__
#define __UNIFORM_BUFFER_OBJECT_HPP__

#include "glad/glad.h"
#include "GLFW/glfw3.h"

class UBO {
    public:
        ~UBO();
        void createUBO(const size_t byteSize, unsigned int usageHint = GL_STREAM_DRAW);
        void bindUBO() const;
        void unbindUBO() const;
        void setBufferData(const size_t offset, const void* ptrData, const size_t dataSize);
        void bindBufferBaseToBindingPoint(const unsigned int bindingPoint);
        unsigned int getBufferID() const { return mBufferID; }
        void deleteUBO();

private:
    unsigned int mBufferID{ 0 };
    size_t mByteSize;

    bool mIsBufferCreated = false;
};

#endif