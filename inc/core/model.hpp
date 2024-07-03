#ifndef __MODEL_HPP__
#define __MODEL_HPP__

#include "core/mesh.hpp"
#include "core/material.hpp"

class Model {
    public:
        Model(std::vector<Mesh> meshes, std::vector<MaterialPtr> mats) : mMeshes(meshes), mMaterials(mats) {}

        void draw(ShaderPtr &shaderProgram) {
            for(auto &mesh : mMeshes) {
                mMaterials[mesh.mMat]->bindMaterial(shaderProgram);
                mesh.draw(shaderProgram);
            }
        }

    private:
        std::vector<Mesh> mMeshes;
        std::vector<MaterialPtr> mMaterials;
};

typedef std::shared_ptr<Model> ModelPtr;

#endif