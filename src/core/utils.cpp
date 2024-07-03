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
    MaterialPtr parseMaterial(const std::string &mp, const tinyobj::material_t &mat) {
        TexturePtr diffuseTexture = nullptr;
        if (!mat.diffuse_texname.empty())
        {
            diffuseTexture = std::make_shared<Texture>(TextureType::DIFFUSE, mp + mat.diffuse_texname);
        }
        TexturePtr specularTexture = nullptr;
        if (!mat.specular_texname.empty())
        {
            specularTexture = std::make_shared<Texture>(TextureType::SPECULAR, mp + mat.specular_texname);
        }
        return std::make_shared<Material>(
            diffuseTexture,
            specularTexture);
    }

    ModelPtr LoadModel(const std::string &fp,
        const std::string &mp) {
            tinyobj::attrib_t attrib;
            std::vector<tinyobj::shape_t> shapes;
            std::vector<tinyobj::material_t> objMaterials;
            std::string err, warn;

            bool success = tinyobj::LoadObj(&attrib, &shapes, &objMaterials, &err, &warn,
            fp.c_str(), mp.c_str());
            if(!err.empty() || !success) {
                std::cerr << err << std::endl;
                return nullptr;
            }
            std::vector<Mesh> meshes;
            for (const auto& shape : shapes) {
                std::unordered_map<Vertex, uint32_t> uniqueVertices;
                std::vector<Vertex> vertices;
                std::vector<int> indices;
                size_t materialId;
                size_t index_offset = 0;
                for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); ++f) {
                    auto fv = static_cast<size_t>(shape.mesh.num_face_vertices[f]);
                    for(size_t v = 0; v < fv; ++v){
                        tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
                        glm::vec3 pos(
                            attrib.vertices[3 * idx.vertex_index + 0],
                            attrib.vertices[3 * idx.vertex_index + 1],
                            attrib.vertices[3 * idx.vertex_index + 2]);
                        glm::vec3 normal(0.0, 0.0, 0.0);
                        if (idx.normal_index >= 0)
                        {
                            normal = glm::vec3(
                                attrib.normals[3 * idx.normal_index + 0],
                                attrib.normals[3 * idx.normal_index + 1],
                                attrib.normals[3 * idx.normal_index + 2]);
                        }
                        glm::vec2 uvs(0.0);
                        if (idx.texcoord_index >= 0)
                        {
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
                    index_offset += fv;
                }
                if(shape.mesh.material_ids[0] < 0) {
                    materialId = 0;
                } else {
                    materialId = shape.mesh.material_ids[0] + 1;
                }
                Mesh mesh(vertices, indices, materialId);
                meshes.push_back(mesh);
            } 
            std::vector<MaterialPtr> materials;
            materials.push_back(std::make_shared<Material>(nullptr, nullptr));
            for(const auto &mat : objMaterials) {
                materials.push_back(
                    parseMaterial(mp, mat)
                );
            }
            return std::make_shared<Model>(meshes, materials);
        }
} // namespace MeshLoader