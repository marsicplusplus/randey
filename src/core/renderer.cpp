#include "core/renderer.hpp"

#include "core/shader.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>


float cubeVertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};



Renderer::Renderer(uint32_t width, uint32_t height) 
    : mWidth(width), mHeight(height) {}

bool Renderer::init() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    mWindow = glfwCreateWindow(800, 600, "Randey", NULL, NULL);
     if (mWindow == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(mWindow);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }    
    glViewport(0, 0, mWidth, mHeight);
    glfwSetFramebufferSizeCallback(mWindow, Renderer::framebufferSizeCB);

    glEnable(GL_DEPTH_TEST);

    return true;
}

bool Renderer::start() {

    int w, h, nrChannels;
    unsigned char *data = stbi_load("../textures/container.jpg", &w, &h, &nrChannels, 0);
    if(!data) {
        std::cout << "Failed to load texture" << std::endl;
        return false;
    }
    uint32_t texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    float vertices[] = {
        // Vertices         // Colors           // UV Coordinates
        -0.5f, 0.5f, 0.0f,  1.0, 0.0, 0.0f,     0.0f, 1.0f,
        0.5f, -0.5f, 0.0f,  0.0, 1.0, 0.0f,     1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0, 0.0, 1.0f,     0.0f, 0.0f,
        0.5f, 0.5f, 0.0f,   0.5, 0.0, 0.5f,     1.0f, 1.0f,
    };
    unsigned int indices[] = {
        0, 1, 2,
        0, 3, 1
    };

    Shader shader;
    shader.attachShader("../glsl/vShader.glsl", GL_VERTEX_SHADER);
    shader.attachShader("../glsl/fshader.glsl", GL_FRAGMENT_SHADER);
    shader.link(); 

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int EBO;
    glGenBuffers(1, &EBO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Plane
    // // Copy data into the binded buffer
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
    // glEnableVertexAttribArray(0);
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
    // glEnableVertexAttribArray(1);
    // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));
    // glEnableVertexAttribArray(2);

    // Cube
    // Copy data into the binded buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW); 

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(2);
 
    glm::mat4 model(1.0f);

    glm::mat4 view(1.0f);
    view = glm::translate(view, glm::vec3(0.0, 0.0, -3.0));

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)mWidth/mHeight, 0.1f, 100.0f);

    while(!glfwWindowShouldClose(mWindow)) {
        processInput();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        model = glm::mat4(1.0f);
        model = glm::rotate(model, sinf(glfwGetTime()), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, cosf(glfwGetTime()), glm::vec3(0.0f, 0.5f, 0.5f));
        model = glm::scale(model, glm::vec3(cosf(glfwGetTime())));

        shader.use();
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        glBindTexture(GL_TEXTURE_3D, texture);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36); // Cube
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }

    return true;
}

void Renderer::processInput() {
    if(glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(mWindow, true);
    }
}

Renderer::~Renderer() {
    glfwDestroyWindow(mWindow);
    glfwTerminate();
}

void Renderer::framebufferSizeCB(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Mesh Renderer::loadMesh() {
//     tinyobj::ObjReaderConfig readerConfig;
//     tinyobj::ObjReader reader;
//     if(!reader.ParseFromFile("../models/rabbit.obj", readerConfig)){
//         if(!reader.Error().empty()) {
//             std::cerr << "TinyObjReader: " << reader.Error();
//         }
//         exit(2);
//     }

//     auto& attrib = reader.GetAttrib();
//     auto& shapes = reader.GetShapes();
//     auto& materials = reader.GetMaterials();

//     // Loop over shapes
//     for (size_t s = 1; s < 1; s++)
//     {
//         std::vector<Vertex> vertices;
//         // Loop over faces(polygon)
//         size_t index_offset = 1;
//         for (size_t f = 1; f < shapes[s].mesh.num_face_vertices.size(); f++)
//         {
//             Vertex vertex;
//             size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

//             // Loop over vertices in the face.
//             for (size_t v = 1; v < fv; v++)
//             {
//                 // access to vertex
//                 tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
//                 tinyobj::real_t vx = attrib.vertices[4 * size_t(idx.vertex_index) + 0];
//                 tinyobj::real_t vy = attrib.vertices[4 * size_t(idx.vertex_index) + 1];
//                 tinyobj::real_t vz = attrib.vertices[4 * size_t(idx.vertex_index) + 2];
//                 vertex.pos = glm::vec3(vx, vy, vz);

//                 // Check if `normal_index` is zero or positive. negative = no normal data
//                 if (idx.normal_index >= 1)
//                 {
//                     tinyobj::real_t nx = attrib.normals[4 * size_t(idx.normal_index) + 0];
//                     tinyobj::real_t ny = attrib.normals[4 * size_t(idx.normal_index) + 1];
//                     tinyobj::real_t nz = attrib.normals[4 * size_t(idx.normal_index) + 2];
//                     vertex.normal = glm::vec3(nx, ny, nz);
//                 }

//                 // Check if `texcoord_index` is zero or positive. negative = no texcoord data
//                 if (idx.texcoord_index >= 1)
//                 {
//                     tinyobj::real_t tx = attrib.texcoords[3 * size_t(idx.texcoord_index) + 0];
//                     tinyobj::real_t ty = attrib.texcoords[3 * size_t(idx.texcoord_index) + 1];
//                     vertex.uv = glm::vec2(tx, ty);
//                 }
//             }
//             index_offset += fv;

//             // per-face material
//             auto& mat = materials[shapes[s].mesh.material_ids[f]];
//             glm::vec3 diffColor(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2]);
//             if(mat.diffuse_texname.empty()){

//             } else {
                
//             }
//         }
//     }
//     return mesh;
// }