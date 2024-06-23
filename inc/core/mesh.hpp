#ifndef __MESH_HPP__
#define __MESH_HPP__

#include "core/texture.hpp"
#include "core/shader.hpp"
#include "core/transform.hpp"
#include "core/material.hpp"

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

#include <vector>

struct Vertex {
    public:
        glm::vec3 mPos;
        glm::vec3 mNormal;
        glm::vec2 mUv;

        Vertex(glm::vec3 pos, glm::vec3 normal, glm::vec2 uv) :
            mPos(pos), mNormal(normal), mUv(uv) {}

        bool operator==(const Vertex& other) const {
            return mPos == other.mPos && mNormal == other.mNormal && mUv == other.mUv;
        }
};

class Mesh {
    public:
        Mesh(const std::vector<Vertex> &vertices, 
                const std::vector<int> &indices,
                Material mat,
                const Transform &t = Transform());
        void draw(Shader &shaderProgram);
        void update(float dt);

        Transform mTransform;
    private:
        void setup();

        unsigned int mVAO, mVBO, mEBO;
        Material mMat;

        std::vector<Vertex> mVertices;
        std::vector<int> mIndices;


};



class CubeMesh : public Mesh {
    CubeMesh();
};

#endif