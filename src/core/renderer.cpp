#include "core/renderer.hpp"
#include "core/camera.hpp"
#include "core/shader.hpp"
#include "managers/input_manager.hpp"
#include "core/utils.hpp"
#include "core/material.hpp"

#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>

 float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };


void APIENTRY glDebugOutput(GLenum source, 
                            GLenum type, 
                            unsigned int id, 
                            GLenum severity, 
                            GLsizei length, 
                            const char *message, 
                            const void *userParam)
{
    // ignore non-significant error/warning codes
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return; 

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " <<  message << std::endl;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break; 
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;
    
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
}

Renderer::Renderer(uint32_t width, uint32_t height) 
    : mWidth(width), mHeight(height) {}

bool Renderer::init() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint (GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
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

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
    glDebugMessageCallback(glDebugOutput, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

    mCamera = std::make_unique<Camera>(glm::vec3(0.0, 2.0, 5.0), glm::vec3(0.0, 1.0, 0.0));
    mProjection = glm::perspective(glm::radians(70.0f), (float)mWidth/mHeight, 0.1f, 100.0f);

    std::vector<Vertex> vertices;
    std::vector<int> indices;
    MaterialPtr material;
    MeshLoader::LoadMesh("C:/Users/loren/OneDrive/Desktop/Code/Randey/models/backpack.obj", "C:/Users/loren/OneDrive/Desktop/Code/Randey/models/", vertices, indices, material);
    Mesh rabbit(vertices, indices, material);

    mMeshes.push_back(rabbit);

    mGeometryShader = std::make_shared<Shader>();
    mGeometryShader->attachShader("C:/Users/loren/OneDrive/Desktop/Code/Randey/glsl/geometry_pass/vShader.glsl", GL_VERTEX_SHADER);
    mGeometryShader->attachShader("C:/Users/loren/OneDrive/Desktop/Code/Randey/glsl/geometry_pass/fShader.glsl", GL_FRAGMENT_SHADER);
    mGeometryShader->link();

    mGBuffer.init(mWidth, mHeight);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    return true;
}

bool Renderer::start() {
    // screen quad VAO
    // unsigned int quadVAO, quadVBO;
    // glGenVertexArrays(1, &quadVAO);
    // glGenBuffers(1, &quadVBO);
    // glBindVertexArray(quadVAO);
    // glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    // glEnableVertexAttribArray(0);
    // glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(1);
    // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    // Shader quadShader;
    // quadShader.attachShader("C:/Users/loren/OneDrive/Desktop/Code/Randey/glsl/screen_quad/vShader.glsl", GL_VERTEX_SHADER);
    // quadShader.attachShader("C:/Users/loren/OneDrive/Desktop/Code/Randey/glsl/screen_quad/fShader.glsl", GL_FRAGMENT_SHADER);
    // quadShader.link(); 

    float deltaTime = 0.0;
    float lastTime = 0.0;

    // glm::vec3 lightColor(1.0);
    // glm::vec3 lightPos(0.0f, 3.0f, 1.0f);

    while(!glfwWindowShouldClose(mWindow)) {
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        processInput();
        double xpos, ypos;
        glfwGetCursorPos(mWindow, &xpos, &ypos);
        InputManager::Instance()->setMouseState(xpos, ypos);
        mCamera->update(deltaTime);

        geometryPass();
        lightPass();

        // objShader.use();
        // objShader.setVec3("light.position", lightPos);
        // objShader.setVec3("light.ambient", lightColor * 0.2f);
        // objShader.setVec3("light.diffuse", lightColor * 0.5f);
        // objShader.setVec3("light.specular", 1.0, 1.0, 1.0);
        // objShader.setMat4("view", view);
        // objShader.setMat4("projection", projection);
        // objShader.setVec3("viewPos", mCamera->getPos());
        // rabbit.draw(objShader);

        // lightShader.use();
        // lightShader.setMat4("view", view);
        // lightShader.setMat4("projection", projection);
        // cube.draw(lightShader);

        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }

    return true;
}

void Renderer::lightPass() {
    glBindFramebuffer(GL_FRAMEBUFFER, mGBuffer.mFbo);

    size_t halfWidth = mWidth / 2.0f;
    size_t halfHeight = mHeight / 2.0f;

    glReadBuffer(GL_COLOR_ATTACHMENT0 + GBufferTexture::DIFFUSE);
    glBlitNamedFramebuffer(mGBuffer.mFbo, 0, 0, 0, mWidth, mHeight, 0, 0, halfWidth, halfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR); // lower left
    
    glReadBuffer(GL_COLOR_ATTACHMENT0 + GBufferTexture::NORMAL);
    glBlitNamedFramebuffer(mGBuffer.mFbo, 0, 0, 0, mWidth, mHeight, halfWidth, halfHeight, mWidth, mHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR); // Upper Right

    glReadBuffer(GL_COLOR_ATTACHMENT0 + GBufferTexture::POSITION);
    glBlitNamedFramebuffer(mGBuffer.mFbo, 0, 0, 0, mWidth, mHeight, 0, halfHeight, halfWidth, mHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR); // Upper Left
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::geometryPass() {
    mGeometryShader->use();
    mGBuffer.bindGeometryPass();
    glDepthMask(GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    mGeometryShader->setMat4("view", mCamera->getViewMatrix());
    mGeometryShader->setMat4("projection", mProjection);
    for(auto &m : mMeshes) {
        m.draw(mGeometryShader);
    }
    glDepthMask(GL_FALSE);
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