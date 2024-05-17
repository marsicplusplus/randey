#ifndef __MESH_HPP__
#define __MESH_HPP__

#include "core/texture.hpp"
#include "core/shader.hpp"

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

#include <vector>

struct Vertex {
    public:
        glm::vec3 pos;
        glm::vec3 normal;
        glm::vec2 uv;
};

class Mesh {
    public:
        Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, const std::vector<Texture> &textures);
        void draw(Shader &shaderProgram);

    private:
        void setup();

        unsigned int mVAO, mVBO, mEBO;

        std::vector<Vertex> mVertices;
        std::vector<unsigned int> mIndices;
        std::vector<Texture> mTextures;

};

#endif