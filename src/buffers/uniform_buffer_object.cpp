#include "buffers/uniform_buffer_object.hpp"

void UBO::createUBO(const size_t byteSize, unsigned usageHint) {
    glGenBuffers(1, &mBufferID);
    bindUBO();
    glNamedBufferData(mBufferID, byteSize, NULL, usageHint);
    mIsBufferCreated = true;
    mByteSize = byteSize;
    unbindUBO();
}

void UBO::bindUBO() const {
    glBindBuffer(GL_UNIFORM_BUFFER, mBufferID);
}

void UBO::unbindUBO() const {
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

UBO::~UBO() {
    deleteUBO();
}

void UBO::deleteUBO() {
    glDeleteBuffers(1, &mBufferID);
    mIsBufferCreated = false;
}

void UBO::bindBufferBaseToBindingPoint(const unsigned int bindingPoint) {
    bindUBO();
    glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, mBufferID);
    unbindUBO();
}

void UBO::setBufferData(const size_t offset, const void* ptrData, const size_t dataSize) {
    bindUBO();
    glNamedBufferSubData(mBufferID, offset, dataSize, ptrData);
    unbindUBO();
}