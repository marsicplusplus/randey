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

        Vertex();
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
        Mesh();

        void setup();

        unsigned int mVAO, mVBO, mEBO;
        unsigned int mMat;

        std::vector<Vertex> mVertices;
        std::vector<int> mIndices;

        friend class Model;
};

class SphereMesh : public Mesh {
    SphereMesh() : Mesh(){
        mVertices.clear();
        mIndices.clear();

        double latitudeBands = 30;
        double longitudeBands = 30;
        double radius = 1;

        for (double latNumber = 0; latNumber <= latitudeBands; latNumber++)
        {
            double theta = latNumber * M_PI / latitudeBands;
            double sinTheta = sin(theta);
            double cosTheta = cos(theta);

            for (double longNumber = 0; longNumber <= longitudeBands; longNumber++)
            {
                double phi = longNumber * 2 * M_PI / longitudeBands;
                double sinPhi = sin(phi);
                double cosPhi = cos(phi);

                Vertex vs;
                vs.mNormal[0] = cosPhi * sinTheta;                   // x
                vs.mNormal[1] = cosTheta;                            // y
                vs.mNormal[2] = sinPhi * sinTheta;                   // z
                vs.mUv[0] = 1 - (longNumber / longitudeBands); // u
                vs.mUv[1] = 1 - (latNumber / latitudeBands);   // v
                vs.mPos[0] = radius * vs.mNormal[0];
                vs.mPos[1] = radius * vs.mNormal[1];
                vs.mPos[2] = radius * vs.mNormal[2];

                mVertices.push_back(vs);
            }

            for (int latNumber = 0; latNumber < latitudeBands; latNumber++)
            {
                for (int longNumber; longNumber < longitudeBands; longNumber++)
                {
                    int first = (latNumber * (longitudeBands + 1)) + longNumber;
                    int second = first + longitudeBands + 1;

                    mIndices.push_back(first);
                    mIndices.push_back(second);
                    mIndices.push_back(first + 1);

                    mIndices.push_back(second);
                    mIndices.push_back(second + 1);
                    mIndices.push_back(first + 1);
                }
            }
        }
        mMat = 0;
        setup();
    }
};

#endif