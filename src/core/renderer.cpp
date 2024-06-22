#include "core/renderer.hpp"
#include "core/camera.hpp"
#include "core/shader.hpp"
#include "managers/input_manager.hpp"
#include "core/utils.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

    glfwSetKeyCallback(this->mWindow, 
    [](GLFWwindow* w, int key, int scancode, int action, int mods) {
		InputManager::Instance()->setKeyValue(key, action != GLFW_RELEASE);
	});


    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    return true;
}

bool Renderer::start() {
    float deltaTime = 0.0;
    float lastTime = 0.0;
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

    std::vector<Vertex> vertices;
    std::vector<int> indices;
    MeshLoader::LoadMesh("../models/rabbit.obj", vertices, indices);
    Mesh rabbit(vertices, indices);

    Shader shader;
    shader.attachShader("../glsl/vShader.glsl", GL_VERTEX_SHADER);
    shader.attachShader("../glsl/fshader.glsl", GL_FRAGMENT_SHADER);
    shader.link(); 

    unsigned int cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Cube
    // Copy data into the binded buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW); 

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(2);
 
    glm::mat4 model(1.0f);

    Camera camera(glm::vec3(0.0, 0.0, 3.0), glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)mWidth/mHeight, 0.1f, 100.0f);

    while(!glfwWindowShouldClose(mWindow)) {
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        processInput();
        double xpos, ypos;
        glfwGetCursorPos(mWindow, &xpos, &ypos);
        InputManager::Instance()->setMouseState(xpos, ypos);
        camera.update(deltaTime);

        glm::mat4 view = camera.getViewMatrix();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        rabbit.draw(shader);

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