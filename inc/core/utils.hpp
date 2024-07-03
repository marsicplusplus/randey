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
        const std::string &mp);
}; // namespace MeshLoader


#endif