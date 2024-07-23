#include "core/renderer.hpp"
#include "core/camera.hpp"
#include "core/shader.hpp"
#include "managers/input_manager.hpp"
#include "core/utils.hpp"
#include "core/material.hpp"

#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>

#define DEBUG false

float quadVertices[] = {
    // positions      
    -1.0f,  1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,
     1.0f, -1.0f, 0.0f
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
    #if DEBUG
    glfwWindowHint (GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    #endif
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

    glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  

    glfwSetKeyCallback(this->mWindow, 
    [](GLFWwindow* w, int key, int scancode, int action, int mods) {
		InputManager::Instance()->setKeyValue(key, action != GLFW_RELEASE);
	});

    #if DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
    glDebugMessageCallback(glDebugOutput, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    #endif

    mCamera = std::make_unique<Camera>(glm::vec3(0.0, 2.0, 5.0), glm::vec3(0.0, 1.0, 0.0));
    mProjection = glm::perspective(glm::radians(70.0f), (float)mWidth/mHeight, 0.1f, 150.0f);

    ModelPtr sponza = MeshLoader::LoadModel("C:/Users/loren/OneDrive/Desktop/Code/Randey/models/sponza/sponza.obj", "C:/Users/loren/OneDrive/Desktop/Code/Randey/models/sponza/", true);
    sponza->getTransform().scale(1.0f/20.0f);
    mModels.push_back(sponza);
    ModelPtr backpack = MeshLoader::LoadModel("C:/Users/loren/OneDrive/Desktop/Code/Randey/models/backpack.obj", "C:/Users/loren/OneDrive/Desktop/Code/Randey/models/", false);
    backpack->getTransform().translate(0.0, 1.4, 0.0);
    backpack->getTransform().scale(0.8f);
    mModels.push_back(backpack);

    mPointLights.push_back(std::make_shared<PointLight>(
        glm::vec3(4.0, 2.0, -1.0),      // Position
        glm::vec3(0.2f, 0.2f, 0.2f),    // Ambient
        glm::vec3(0.5f, 0.5f, 0.5f)     // Diffuse
    ));
    mPointLights.push_back(std::make_shared<PointLight>(
        glm::vec3(-4.0, 4.0, -1.0),      // Position
        glm::vec3(0.2f, 0.2f, 0.2f),    // Ambient
        glm::vec3(1.0f, 0.5f, 0.5f)     // Diffuse
    ));

    // mDirLights.push_back(std::make_shared<DirectionalLight>(
    //     glm::vec3(-0.2f, -1.0f, -0.3f),    // direction
    //     glm::vec3(0.2, 0.2, 0.2),    // Ambient
    //     glm::vec3(0.4, 0.3, 0.3)     // Diffuse
    // ));

    mGeometryShader = std::make_shared<Shader>();
    mGeometryShader->attachShader("C:/Users/loren/OneDrive/Desktop/Code/Randey/glsl/geometry_pass/vShader.glsl", GL_VERTEX_SHADER);
    mGeometryShader->attachShader("C:/Users/loren/OneDrive/Desktop/Code/Randey/glsl/geometry_pass/fShader.glsl", GL_FRAGMENT_SHADER);
    mGeometryShader->link();

    mPointLightsShader = std::make_shared<Shader>();
    mPointLightsShader->attachShader("C:/Users/loren/OneDrive/Desktop/Code/Randey/glsl/light_pass/vShader.glsl", GL_VERTEX_SHADER);
    mPointLightsShader->attachShader("C:/Users/loren/OneDrive/Desktop/Code/Randey/glsl/light_pass/fShader_point_light.glsl", GL_FRAGMENT_SHADER);
    mPointLightsShader->link();

    mDirectionalLightsShader = std::make_shared<Shader>();
    mDirectionalLightsShader->attachShader("C:/Users/loren/OneDrive/Desktop/Code/Randey/glsl/light_pass/vShader.glsl", GL_VERTEX_SHADER);
    mDirectionalLightsShader->attachShader("C:/Users/loren/OneDrive/Desktop/Code/Randey/glsl/light_pass/fShader_dir_light.glsl", GL_FRAGMENT_SHADER);
    mDirectionalLightsShader->link();


    mGBuffer.init(mWidth, mHeight);

    glGenVertexArrays(1, &mQuadVAO);
    glGenBuffers(1, &mQuadVBO);
    glBindVertexArray(mQuadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mQuadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    return true;
}

bool Renderer::start() {
    float deltaTime = 0.0;
    float lastTime = 0.0;

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

        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }

    return true;
}

void Renderer::lightPass() {
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);

    mGBuffer.bindLightPass();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    pointLightsPass();
    directionalLightsPass();
}

void Renderer::pointLightsPass() {
    mPointLightsShader->use();
    mPointLightsShader->setVec2("gScreenSize", mWidth, mHeight);
    mPointLightsShader->setVec3("gViewPos", mCamera->getPos());
    for(auto &l : mPointLights) {
        l->bindLight(mPointLightsShader);       
        glBindVertexArray(mQuadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
}

void Renderer::directionalLightsPass() {
    mDirectionalLightsShader->use();
    mDirectionalLightsShader->setVec2("gScreenSize", mWidth, mHeight);
    mDirectionalLightsShader->setVec3("gViewPos", mCamera->getPos());
    for(auto &l : mDirLights) {
        l->bindLight(mDirectionalLightsShader);       
        glBindVertexArray(mQuadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
}

void Renderer::geometryPass() {
    mGeometryShader->use();
    mGBuffer.bindGeometryPass();
    glDepthMask(GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    mGeometryShader->setMat4("view", mCamera->getViewMatrix());
    mGeometryShader->setMat4("projection", mProjection);
    for(auto &m : mModels) {
        m->draw(mGeometryShader);
    }
    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);
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