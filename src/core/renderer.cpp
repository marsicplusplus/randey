#include "core/renderer.hpp"
#include "core/camera.hpp"
#include "core/shader.hpp"
#include "managers/input_manager.hpp"
#include "core/utils.hpp"
#include "core/material.hpp"

#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <exception>

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
    if(type == GL_DEBUG_TYPE_PERFORMANCE) return;

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

bool Renderer::init(const std::string &filePath) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    #if DEBUG
    glfwWindowHint (GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    #endif
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    mWindow = glfwCreateWindow(mWidth, mHeight, "Randey", NULL, NULL);
     if (mWindow == nullptr) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW Window.");
    }

    glfwMakeContextCurrent(mWindow);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLAD");
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

    mSphereMesh = std::make_shared<SphereMesh>();
    mCubeMesh = std::make_shared<CubeMesh>();

    std::vector<ModelPtr> models;

    SceneParser::parseScene(filePath, 
        mModels, 
        mDirLights, 
        mPointLights, 
        mSkyboxTexture, 
        &mCamera,  
        mWidth, mHeight);

    mProjection = glm::perspective(glm::radians(70.0f), (float)mWidth/mHeight, 0.1f, 500.0f);

    mLightRenderingShader = std::make_shared<Shader>();
    mLightRenderingShader->attachShader("glsl/lightVShader.glsl", GL_VERTEX_SHADER);
    mLightRenderingShader->attachShader("glsl/lightFShader.glsl", GL_FRAGMENT_SHADER);
    mLightRenderingShader->link();

    mStencilPassShader = std::make_shared<Shader>();
    mStencilPassShader->attachShader("glsl/stencil_pass/vShader.glsl", GL_VERTEX_SHADER);
    mStencilPassShader->attachShader("glsl/stencil_pass/fShader.glsl", GL_FRAGMENT_SHADER);
    mStencilPassShader->link();

    mGeometryShader = std::make_shared<Shader>();
    mGeometryShader->attachShader("glsl/geometry_pass/vShader.glsl", GL_VERTEX_SHADER);
    mGeometryShader->attachShader("glsl/geometry_pass/fShader.glsl", GL_FRAGMENT_SHADER);
    mGeometryShader->link();

    mDirectionalShadowMapShader = std::make_shared<Shader>();
    mDirectionalShadowMapShader->attachShader("glsl/directional_shadow_pass/vShader.glsl", GL_VERTEX_SHADER);
    mDirectionalShadowMapShader->attachShader("glsl/directional_shadow_pass/fShader.glsl", GL_FRAGMENT_SHADER);
    mDirectionalShadowMapShader->link();

    mPointShadowMapShader = std::make_shared<Shader>();
    mPointShadowMapShader->attachShader("glsl/point_shadow_pass/vShader.glsl", GL_VERTEX_SHADER);
    mPointShadowMapShader->attachShader("glsl/point_shadow_pass/gShader.glsl", GL_GEOMETRY_SHADER);
    mPointShadowMapShader->attachShader("glsl/point_shadow_pass/fShader.glsl", GL_FRAGMENT_SHADER);
    mPointShadowMapShader->link();

    mPointLightsShader = std::make_shared<Shader>();
    mPointLightsShader->attachShader("glsl/light_pass/vShader.glsl", GL_VERTEX_SHADER);
    mPointLightsShader->attachShader("glsl/light_pass/fShader_point_light.glsl", GL_FRAGMENT_SHADER);
    mPointLightsShader->link();

    mDirectionalLightsShader = std::make_shared<Shader>();
    mDirectionalLightsShader->attachShader("glsl/light_pass/vShader.glsl", GL_VERTEX_SHADER);
    mDirectionalLightsShader->attachShader("glsl/light_pass/fShader_dir_light.glsl", GL_FRAGMENT_SHADER);
    mDirectionalLightsShader->link();

    mSkyboxShader = std::make_shared<Shader>();
    mSkyboxShader->attachShader("glsl/skybox/vShader.glsl", GL_VERTEX_SHADER);
    mSkyboxShader->attachShader("glsl/skybox/fShader.glsl", GL_FRAGMENT_SHADER);
    mSkyboxShader->link();

    mTransparencyShader = std::make_shared<Shader>();
    mTransparencyShader->attachShader("glsl/transparency/vShader.glsl", GL_VERTEX_SHADER);
    mTransparencyShader->attachShader("glsl/transparency/fShader.glsl", GL_FRAGMENT_SHADER);
    mTransparencyShader->link();

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

        shadowMapPass();


        glViewport(0, 0, mWidth, mHeight);
        mGBuffer.startFrame();

        geometryPass();
        lightPass();

        mGBuffer.bindForwardPass();
        drawPointLights();
        transparencyPass();
        skyboxPass();

        mGBuffer.bindFinalPass();
        glBlitFramebuffer(0, 0, mWidth, mHeight, 
                          0, 0, mWidth, mHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);


        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }

    return true;
}

void Renderer::transparencyPass() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
    mTransparencyShader->use();
    mTransparencyShader->setMat4("projection", mProjection);
    mTransparencyShader->setMat4("view", mCamera->getViewMatrix());
    for(auto &m : mModels) {
        m->draw(mTransparencyShader, true);
    }
}

void Renderer::skyboxPass() {
    if(mSkyboxTexture != nullptr) {
        glDisable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        mSkyboxShader->use();

        int prevCullMode;
        glGetIntegerv(GL_CULL_FACE_MODE, &prevCullMode);
        int prevDepthMode;
        glGetIntegerv(GL_DEPTH_FUNC, &prevDepthMode);

        glCullFace(GL_FRONT);
        glDepthFunc(GL_LEQUAL);

        mSkyboxShader->setMat4("gProjection", mProjection);
        mSkyboxShader->setMat4("gView", mCamera->getViewMatrix());
        mSkyboxTexture->bind(0);
        mCubeMesh->draw(mSkyboxShader);

        glCullFace(prevCullMode);
        glDepthFunc(prevDepthMode);
    }
}

void Renderer::shadowMapPass() {
    glCullFace(GL_FRONT);
    mDirectionalShadowMapShader->use();
    for (auto d : mDirLights) {
        if(d->isShadowCaster()){ 
            d->bindForShadowPass(mDirectionalShadowMapShader);
            glClear(GL_DEPTH_BUFFER_BIT);
            for(auto &m : mModels) {
                m->draw(mDirectionalShadowMapShader);
            }
        }
    }

    mPointShadowMapShader->use();
    for (auto d : mPointLights) {
        if(d->isShadowCaster()){
            d->bindForShadowPass(mPointShadowMapShader);
            glClear(GL_DEPTH_BUFFER_BIT);
            for(auto &m : mModels) {
                m->draw(mPointShadowMapShader);
            }
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glCullFace(GL_BACK);
}

void Renderer::drawPointLights() {
    glEnable(GL_DEPTH_TEST);
    // Draw Lights as Spheres
    if(mDrawLights) {
        mLightRenderingShader->use();
        auto view = this->mCamera->getViewMatrix();
        mLightRenderingShader->setMat4("view", view);
        mLightRenderingShader->setMat4("projection", mProjection);
        for(const auto l : mPointLights) {
            auto lightPos = l->getPosition();
            Transform t;
            t.translate(lightPos.x, lightPos.y, lightPos.z);
            t.scale(0.25); // scale down unit sphere
            mLightRenderingShader->setMat4("model", t.getMatrix());
            mLightRenderingShader->setVec3("diffuse", l->getDiffuse());
            mSphereMesh->draw(mLightRenderingShader);
        }
    }
}

void Renderer::lightPass() {
    pointLightsPass();
    directionalLightsPass();
}

void Renderer::pointLightsPass() {
    glEnable(GL_STENCIL_TEST);
    for(auto &l : mPointLights) {
        float volumeScale = l->getVolumeRadius();
        glm::vec3 pos = l->getPosition();
        Transform t;
        t.scale(volumeScale);
        t.translate(pos);
        // Stencil Pass:
        mStencilPassShader->use();
        mGBuffer.bindStencilPass();
        mStencilPassShader->setMat4("projection", mProjection);
        mStencilPassShader->setMat4("view", mCamera->getViewMatrix());
        mStencilPassShader->setMat4("model", t.getMatrix());

        glEnable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glClear(GL_STENCIL_BUFFER_BIT);
        glStencilFunc(GL_ALWAYS, 0, 0);
        glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
        glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
        mSphereMesh->draw(mStencilPassShader);

        // Light Pass:
        glEnable(GL_BLEND);
        glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
        glDisable(GL_DEPTH_TEST);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_ONE, GL_ONE);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);

        mPointLightsShader->use();
        mGBuffer.bindLightPass();
        mPointLightsShader->setVec2("gScreenSize", mWidth, mHeight);
        mPointLightsShader->setVec3("gViewPos", mCamera->getPos());
        mPointLightsShader->setMat4("projection", mProjection);
        mPointLightsShader->setMat4("view", mCamera->getViewMatrix());
        mPointLightsShader->setMat4("model", t.getMatrix());

        l->bindLight(mPointLightsShader);
        mSphereMesh->draw(mPointLightsShader);
        glCullFace(GL_BACK);
        glDisable(GL_BLEND);
    }
    glDisable(GL_STENCIL_TEST);
}

void Renderer::directionalLightsPass() {
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);

    mGBuffer.bindLightPass();
    glm::mat4 identity(1.0f);
    mDirectionalLightsShader->use();
    mDirectionalLightsShader->setVec2("gScreenSize", mWidth, mHeight);
    mDirectionalLightsShader->setVec3("gViewPos", mCamera->getPos());
    mDirectionalLightsShader->setMat4("projection", identity);
    mDirectionalLightsShader->setMat4("view", identity);

    for(auto &l : mDirLights) {
        l->bindLight(mDirectionalLightsShader);       
        mDirectionalLightsShader->setMat4("model", identity);
        glBindVertexArray(mQuadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
    glDisable(GL_BLEND);
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
}

void Renderer::processInput() {
    if(glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(mWindow, true);
    }
}

Renderer::~Renderer() {
    glfwDestroyWindow(mWindow);
    glfwTerminate();
    delete mCamera;
}

void Renderer::framebufferSizeCB(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}