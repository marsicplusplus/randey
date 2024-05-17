#include "core/mesh.hpp"

#include "glad/glad.h"

#include <string>

Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, const std::vector<Texture> &textures) {
    this->mIndices = indices;
    this->mTextures = textures;
    this->mVertices = vertices;

    setup();
}

void Mesh::setup() {
    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);
    glGenBuffers(1, &mEBO);

    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), &mVertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int),
                 &mIndices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));

    glBindVertexArray(0);
}

void Mesh::draw(Shader &shaderProgram) {
    uint32_t diffuseNr = 1;
    uint32_t specularNr = 1;
    for(uint32_t i = 0; i < mTextures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        std::string name;
        if(mTextures[i].getType() == TextureType::DIFFUSE) {
            name = "texture_diffuse";
            number = std::to_string(diffuseNr++);
        } else {
            name = "texture_specular";
            number = std::to_string(specularNr++);
        }
        shaderProgram.setInt(("material." + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, mTextures[i].getId());
    }
    glActiveTexture(0);

    glBindVertexArray(mVAO);
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}