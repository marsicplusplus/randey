#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <string>
#include <vector>

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

#include "core/mesh.hpp"

namespace MeshLoader
{
    bool LoadMesh(const std::string &fp, 
        std::vector<Vertex> &vertices,
        std::vector<int> &indices);
} // namespace MeshLoader


#endif