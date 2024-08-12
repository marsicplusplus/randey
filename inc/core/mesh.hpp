#ifndef __MESH_HPP__
#define __MESH_HPP__

#include "core/texture.hpp"
#include "core/shader.hpp"
#include "core/transform.hpp"
#include "core/material.hpp"

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

#include <vector>

#define PI       3.14159265358979323846   // pi

struct Vertex {
    public:
        glm::vec3 mPos;
        glm::vec3 mNormal;
        glm::vec2 mUv;

        Vertex() {};
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
                size_t materialId);
        void draw(ShaderPtr &shaderProgram);
        void update(float dt);

    protected:
        Mesh() {};

        void setup();

        unsigned int mVAO, mVBO, mEBO;
        unsigned int mMat;

        std::vector<Vertex> mVertices;
        std::vector<int> mIndices;

        friend class Model;
};

static float CubeVertices[] = {
            // positions
            -1.0f, 1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,

            -1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,

            -1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, 1.0f
    };

class CubeMesh : public Mesh {
    public:
        CubeMesh() : Mesh() {
            glGenVertexArrays(1, &mVAO);
            glGenBuffers(1, &mVBO);

            glBindVertexArray(mVAO);
            glBindBuffer(GL_ARRAY_BUFFER, mVBO);

            glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVertices), &CubeVertices[0], GL_STATIC_DRAW);

            // vertex positions
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

            glBindVertexArray(0);
        }

        void draw(ShaderPtr &shaderProgram) {
            glBindVertexArray(mVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
        } 
};
        
class SphereMesh : public Mesh {
    public: 
        SphereMesh() : Mesh(){
            mVertices.clear();
            mIndices.clear();
            const int sectorCount = 20;
            const int stackCount = 20;

            float x, y, z, xy;                           // vertex position
            float nx, ny, nz, lengthInv = 1.0f; // vertex normal
            float s, t;                                  // vertex texCoord

            float sectorStep = 2 * PI / sectorCount;
            float stackStep = PI / stackCount;
            float sectorAngle, stackAngle;

            for (int i = 0; i <= stackCount; ++i) {
                stackAngle = PI / 2 - i * stackStep; // starting from pi/2 to -pi/2
                xy = cosf(stackAngle);      // r * cos(u)
                z = sinf(stackAngle);       // r * sin(u)

                // add (sectorCount+1) vertices per stack
                // first and last vertices have same position and normal, but different tex coords
                for (int j = 0; j <= sectorCount; ++j) {
                    sectorAngle = j * sectorStep; // starting from 0 to 2pi

                    // vertex position (x, y, z)
                    x = xy * cosf(sectorAngle); // r * cos(u) * cos(v)
                    y = xy * sinf(sectorAngle); // r * cos(u) * sin(v)
                    glm::vec3 pos(x, y, z);

                    // normalized vertex normal (nx, ny, nz)
                    nx = x * lengthInv;
                    ny = y * lengthInv;
                    nz = z * lengthInv;
                    glm::vec3 norm(nx, ny, nz);

                    // vertex tex coord (s, t) range between [0, 1]
                    s = (float)j / sectorCount;
                    t = (float)i / stackCount;
                    glm::vec2 uv(s, t);
                    mVertices.push_back(Vertex(pos, norm, uv));
                }
            }

            int k1, k2;
            for (int i = 0; i < stackCount; ++i) {
                k1 = i * (sectorCount + 1); // beginning of current stack
                k2 = k1 + sectorCount + 1;  // beginning of next stack

                for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
                    // 2 triangles per sector excluding first and last stacks
                    // k1 => k2 => k1+1
                    if (i != 0) {
                        mIndices.push_back(k1);
                        mIndices.push_back(k2);
                        mIndices.push_back(k1 + 1);
                    }

                    // k1+1 => k2 => k2+1
                    if (i != (stackCount - 1)) {
                        mIndices.push_back(k1 + 1);
                        mIndices.push_back(k2);
                        mIndices.push_back(k2 + 1);
                    }
                }
            }

            mMat = 0;
            setup();
        }
};

#endif