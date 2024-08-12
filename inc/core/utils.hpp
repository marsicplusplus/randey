#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <string>
#include <vector>

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

#include "core/mesh.hpp"
#include "core/model.hpp"
#include "core/material.hpp"

namespace MeshLoader
{
    ModelPtr LoadModel(const std::string &fp, 
        const std::string &mp, bool flipTexture = false);
}; // namespace MeshLoader

namespace Images{
    unsigned char* loadImage(const std::string &fileName, int &width, int &height, int &nrChannel);
    void freeData(unsigned char* data);
};

#endif