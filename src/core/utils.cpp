#include "core/utils.hpp"
#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

namespace std {
    template<> struct hash<Vertex> {
        size_t operator()(Vertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.mPos) ^
                   (hash<glm::vec3>()(vertex.mNormal) << 1)) >> 1) ^
                   (hash<glm::vec2>()(vertex.mUv) << 1);
        }
    };
}

namespace MeshLoader
{
    bool LoadMesh(const std::string &fp, 
        std::vector<Vertex> &vertices,
        std::vector<int> &indices) {
            tinyobj::attrib_t attrib;
            std::vector<tinyobj::shape_t> shapes;
            std::vector<tinyobj::material_t> objMaterials;
            std::string err, warn;

            bool success = tinyobj::LoadObj(&attrib, &shapes, &objMaterials, &err, &warn,
            fp.c_str());
            if(!err.empty() || !success) {
                std::cerr << err << std::endl;
                return false;
            }
            std::unordered_map<Vertex, uint32_t> uniqueVertices;
            for (const auto& shape : shapes) {
                for(const auto& idx : shape.mesh.indices) {
                    glm::vec3 pos(
                            attrib.vertices[3 * idx.vertex_index + 0],
                            attrib.vertices[3 * idx.vertex_index + 1],
                            attrib.vertices[3 * idx.vertex_index + 2]);
                    glm::vec3 normal(0.0, 0.0, 0.0);
                    if(idx.normal_index >= 0) {
                        normal = glm::vec3(
                            attrib.normals[3 * idx.normal_index + 0],
                            attrib.normals[3 * idx.normal_index + 1],
                            attrib.normals[3 * idx.normal_index + 2]);
                    }
                    glm::vec2 uvs(0.0);
                    if(idx.texcoord_index >= 0) {
                        uvs = glm::vec2(
                                attrib.texcoords[2 * idx.texcoord_index + 0],
                                attrib.texcoords[2 * idx.texcoord_index + 1]);
                    }

                    Vertex vertex(pos, normal, uvs);
                    if(uniqueVertices.count(vertex) == 0) {
                        uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                        vertices.push_back(vertex);
                    }
                    indices.push_back(uniqueVertices[vertex]);
                }
            }
            return true;
        }
  
} // namespace MeshLoader