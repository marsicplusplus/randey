#ifndef __MODEL_HPP__
#define __MODEL_HPP__

#include "core/mesh.hpp"
#include "core/material.hpp"
#include "managers/resource_manager.hpp"

class Model {
    public:
        Model(std::vector<Mesh> meshes, std::vector<MaterialPtr> mats) : mMeshes(meshes), mMaterials(mats) {}
        Transform& getTransform() { return mTransform; }

        void draw(ShaderPtr &shaderProgram, bool drawTransparent = false) {
            shaderProgram->setMat4("model", mTransform.getMatrix());
            shaderProgram->setMat4("modelTransposeInverse", mTransform.getTransposeInverse());
            for(auto &mesh : mMeshes) {
                auto mat = mMaterials[mesh.mMat];
                 if((drawTransparent && mat->getTexture(TextureType::ALPHA) == 0) 
                    || (!drawTransparent && mat->getTexture(TextureType::ALPHA) != 0))
                    continue;
                mat->bindMaterial(shaderProgram);
                mesh.draw(shaderProgram);
            }
        }

    private:
        Transform mTransform;
        std::vector<Mesh> mMeshes;
        std::vector<MaterialPtr> mMaterials;
};

typedef std::shared_ptr<Model> ModelPtr;

#endif