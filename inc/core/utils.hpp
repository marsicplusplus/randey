#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <string>
#include <vector>
#include <toml++/toml.hpp>

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

#include "core/mesh.hpp"
#include "core/model.hpp"
#include "core/material.hpp"
#include "lights/point_light.hpp"
#include "lights/directional_light.hpp"
#include "core/camera.hpp"
#include "core/cubemap_texture.hpp"

namespace SceneParser {
    bool parseScene(std::string filename, std::vector<ModelPtr> &models,
                                            std::vector<DirectionalLightPtr> &dirLights,
                                            std::vector<PointLightPtr> &pointLights,
                                            std::shared_ptr<CubemapTexture> &cubeMap,
                                            Camera **camera,
                                            uint32_t &width, uint32_t &height);
};

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