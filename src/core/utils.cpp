#include "core/utils.hpp"
#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "managers/resource_manager.hpp"

#include "stb_image.h"
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
    MaterialPtr parseMaterial(const std::string &mp, const tinyobj::material_t &mat, bool flipTexture) {
        unsigned int diffuseTexture = 0;
        if (!mat.diffuse_texname.empty())
        {
            diffuseTexture = ResourceManager::Instance()->setTexture(mp + mat.diffuse_texname, flipTexture);
        }
        unsigned int specularTexture = 0;
        if (!mat.specular_texname.empty())
        {
            specularTexture = ResourceManager::Instance()->setTexture(mp + mat.specular_texname, flipTexture);
        }
        unsigned int ambientTexture = 0;
        if (!mat.ambient_texname.empty())
        {
            ambientTexture = ResourceManager::Instance()->setTexture(mp + mat.ambient_texname, flipTexture);
        }
        unsigned int alphaTexture = 0;
        if (!mat.alpha_texname.empty())
        {
            alphaTexture = ResourceManager::Instance()->setTexture(mp + mat.alpha_texname, flipTexture);
        }
        return std::make_shared<Material>(
            diffuseTexture,
            specularTexture,
            ambientTexture,
            alphaTexture);
    }

    ModelPtr LoadModel(const std::string &fp,
        const std::string &mp,
        bool flipTexture) {
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
            materials.push_back(std::make_shared<Material>(0, 0, 0, 0));
            for(const auto &mat : objMaterials) {
                materials.push_back(
                    parseMaterial(mp, mat, flipTexture)
                );
            }
            return std::make_shared<Model>(meshes, materials);
        }
} // namespace MeshLoader

namespace Images {
    unsigned char* loadImage(const std::string &fileName, int &width, int &height, int &nrChannels) {
        unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &nrChannels, 0);
        if(data == nullptr){
            return nullptr;
        }
        return data;
    }

    void freeData(unsigned char *data) {
        stbi_image_free(data);
    }
}

namespace SceneParser {
    glm::vec3 parseVec3(const toml::node_view<toml::node> tbl) {
        glm::vec3 ret(
            tbl["x"].value_or(0.0),
            tbl["y"].value_or(0.0),
            tbl["z"].value_or(0.0)
        );
        return ret;
    }
    glm::vec3 parseColor(const toml::node_view<toml::node> tbl) {
        glm::vec3 ret(
            tbl["r"].value_or(0.0),
            tbl["g"].value_or(0.0),
            tbl["b"].value_or(0.0)
        );
        return ret;
    }

    bool parseCamera(const toml::node_view<toml::node> tbl, Camera *camera) {
                return true;
    }

    bool parseScene(std::string filename, std::vector<ModelPtr> &models,
                                            std::vector<DirectionalLightPtr> &dirLights,
                                            std::vector<PointLightPtr> &pointLights,
                                            std::shared_ptr<CubemapTexture> &cubeMap,
                                            Camera **camera,
                                            uint32_t &width, uint32_t &height) {
        toml::table tbl;
        try {
            tbl = toml::parse_file(filename);
            // std::cout << tbl << "\n";
        }
        catch (const toml::parse_error &err) {
            std::cerr << "Scene parsing failed:\n"
                      << err << "\n";
            return false;
        }

        width = tbl["options"]["width"].value_or(800);
        height = tbl["options"]["height"].value_or(600);

        models.clear();
        dirLights.clear();
        pointLights.clear();

        auto cameraToml = tbl["camera"];
        if(!cameraToml.is_table()) {
            std::cerr << "No camera in the toml file." << std::endl;
            return false;
        }

        glm::vec3 pos = parseVec3(cameraToml["position"]);
        glm::vec3 dir = parseVec3(cameraToml["direction"]);
        glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
        *camera = new Camera(pos, dir, up); 

        auto modelsTable = tbl["models"];
        toml::array *modelsArray = modelsTable.as_array();
        if(modelsArray == nullptr) {
            std::cerr << "No models in scene file\n";
            return false;
        }
        modelsArray->for_each([&](toml::table &elem) { 
            const std::string path = elem["path"].value_or("");
            const std::string materialPath = elem["material_dir"].value_or("");
            const bool flipTexture = elem["flip_texture"].value_or(false);
            const glm::vec3 scale = parseVec3(elem["scale"]);
            const glm::vec3 translate = parseVec3(elem["translate"]);
            const glm::vec3 rotate = parseVec3(elem["rotate"]);
            Transform t = Transform(scale, rotate, translate);
            std::cout << "path: " << path << '\n';
            ModelPtr model = MeshLoader::LoadModel(path, materialPath, flipTexture);
            model->setTransform(t);
            models.push_back(model);
        });

        // Lights:

        auto pointLightsTable = tbl["point_lights"];
        toml::array *pointLightsArray = pointLightsTable.as_array();
        if(pointLightsArray != nullptr) {
            pointLightsArray->for_each([&](toml::table &elem) { 
                const glm::vec3 position = parseVec3(elem["position"]);
                const glm::vec3 ambient = parseColor(elem["ambient"]);
                const glm::vec3 diffuse = parseColor(elem["diffuse"]);
                pointLights.push_back(std::make_shared<PointLight>(
                    position,
                    ambient,
                    diffuse
                ));
            });
        }

        auto dirLightsTable = tbl["directional_lights"];
        toml::array *dirLightsArray = dirLightsTable.as_array();
        if(dirLightsArray != nullptr) {
            dirLightsArray->for_each([&](toml::table &elem) { 
                const glm::vec3 direction = parseVec3(elem["direction"]);
                const glm::vec3 ambient = parseColor(elem["ambient"]);
                const glm::vec3 diffuse = parseColor(elem["diffuse"]);
                ShadowMapFBOPtr shadowMap = std::make_unique<ShadowMapFBO>();
                shadowMap->init(width, height, GL_TEXTURE_2D);
                dirLights.push_back(std::make_shared<DirectionalLight>(
                    direction,
                    ambient,
                    diffuse,
                    std::move(shadowMap)
                ));
            });
        }

        // TODO: Add check for missing images
        if(!tbl["skybox"].is_table()){
            cubeMap = nullptr;
        } else {
            auto skybox = tbl["skybox"];
            cubeMap = std::make_shared<CubemapTexture>( skybox["pos_x"].value_or(""),
                                                        skybox["neg_x"].value_or(""),
                                                        skybox["pos_y"].value_or(""),
                                                        skybox["neg_y"].value_or(""),
                                                        skybox["pos_z"].value_or(""),
                                                        skybox["neg_z"].value_or("")
                                                    );
        }

       return true;
    }
}