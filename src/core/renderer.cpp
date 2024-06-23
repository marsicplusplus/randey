#include "core/renderer.hpp"
#include "core/camera.hpp"
#include "core/shader.hpp"
#include "managers/input_manager.hpp"
#include "core/utils.hpp"
#include "core/material.hpp"

#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>

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

    mCamera = std::make_unique<Camera>(glm::vec3(0.0, 3.0, 5.0), glm::vec3(0.0, 1.0, 0.0));
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    return true;
}

bool Renderer::start() {
    float deltaTime = 0.0;
    float lastTime = 0.0;

    std::vector<Vertex> vertices;
    std::vector<int> indices;
    std::vector<Material> materials;
    std::vector<int> materialIdxs;
    MeshLoader::LoadMesh("../models/backpack.obj", "../models/", vertices, indices, materials, materialIdxs);
    Material material(
        glm::vec3(1.0, 0.5, 0.31),
        nullptr,
        glm::vec3(1.0, 0.5, 0.31),
        nullptr,
        glm::vec3(1.0, 1.0, 1.0),
        nullptr
    );
    Mesh rabbit(vertices, indices, material);

    vertices.clear();
    indices.clear();
    materials.clear();
    materialIdxs.clear();
    MeshLoader::LoadMesh("../models/cube.obj", "../models/", vertices, indices, materials, materialIdxs);
    Material lightMaterial(
        glm::vec3(1.0, 1.0, 1.0),
        nullptr,
        glm::vec3(1.0, 1.0, 1.0),
        nullptr,
        glm::vec3(1.0, 1.0, 1.0),
        nullptr
    );
    Mesh cube(vertices, indices, lightMaterial);

    Shader objShader;
    objShader.attachShader("../glsl/vShader.glsl", GL_VERTEX_SHADER);
    objShader.attachShader("../glsl/fshader.glsl", GL_FRAGMENT_SHADER);
    objShader.link(); 

    Shader lightShader;
    lightShader.attachShader("../glsl/vShader.glsl", GL_VERTEX_SHADER);
    lightShader.attachShader("../glsl/lightFShader.glsl", GL_FRAGMENT_SHADER);
    lightShader.link(); 

    glm::mat4 projection = glm::perspective(glm::radians(70.0f), (float)mWidth/mHeight, 0.1f, 100.0f);

    glm::vec3 lightColor(1.0);
    glm::vec3 lightPos(0.0f, 3.0f, 1.0f);

    cube.mTransform.translate(lightPos);
    cube.mTransform.scale(0.2);

    rabbit.mTransform.translate(0.0, -0.0, 1.0);
    rabbit.mTransform.scale(5);

    while(!glfwWindowShouldClose(mWindow)) {
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        processInput();
        double xpos, ypos;
        glfwGetCursorPos(mWindow, &xpos, &ypos);
        InputManager::Instance()->setMouseState(xpos, ypos);
        mCamera->update(deltaTime);

        glm::mat4 view = mCamera->getViewMatrix();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::vec3 lightColor {
            sin(glfwGetTime() * 2.0f),
            sin(glfwGetTime() * 0.7f),
            sin(glfwGetTime() * 1.3f),
        };
        objShader.use();
        objShader.setVec3("light.position", lightPos);
        objShader.setVec3("light.ambient", lightColor * 0.2f);
        objShader.setVec3("light.diffuse", lightColor * 0.5f);
        objShader.setVec3("light.specular", 1.0, 1.0, 1.0);
        objShader.setMat4("view", view);
        objShader.setMat4("projection", projection);
        objShader.setVec3("viewPos", mCamera->getPos());
        rabbit.draw(objShader);

        lightShader.use();
        lightShader.setMat4("view", view);
        lightShader.setMat4("projection", projection);
        cube.draw(lightShader);

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