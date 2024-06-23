#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <string>
#include <vector>

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

#include "core/mesh.hpp"
#include "core/material.hpp"

namespace MeshLoader
{
    bool LoadMesh(const std::string &fp, 
        const std::string &mp,
        std::vector<Vertex> &vertices,
        std::vector<int> &indices,
        std::vector<Material> &materials,
        std::vector<int> &materialIdxs);
} // namespace MeshLoader


#endif