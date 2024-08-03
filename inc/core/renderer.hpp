#ifndef __RENDERER_HPP__
#define __RENDERER_HPP__

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "core/camera.hpp"
#include "core/mesh.hpp"
#include "core/model.hpp"
#include "lights/point_light.hpp"
#include "lights/directional_light.hpp"
#include "buffers/gbuffer.hpp"
#include "buffers/uniform_buffer_object.hpp"

#include <memory>

class Renderer {
    public:
        Renderer(uint32_t width, uint32_t height);
        ~Renderer();

        bool init();
        bool start();
        
    private:
        GLFWwindow *mWindow;
        uint32_t mWidth, mHeight;
        std::unique_ptr<Camera> mCamera;
        std::vector<ModelPtr> mModels;
        std::shared_ptr<SphereMesh> mSphereMesh;
        std::vector<PointLight> mPointLights;
        std::vector<DirectionalLight> mDirLights;
        ShaderPtr mGeometryShader;
        ShaderPtr mPointLightsShader;
        ShaderPtr mDirectionalLightsShader;
        ShaderPtr mLightRenderingShader;
        ShaderPtr mStencilPassShader;
        ShaderPtr mTransparencyShader;
        std::unique_ptr<UBO> mPointLightsUBO;
        std::unique_ptr<UBO> mDirLightsUBO;

        GBuffer mGBuffer;
        glm::mat4 mProjection;

        // Quad
        uint32_t mQuadVAO;
        uint32_t mQuadVBO;

        bool mDrawLights = true;

        static void framebufferSizeCB(GLFWwindow *window, int width, int height);
        void processInput();
        void geometryPass();
        void lightPass();
        void pointLightsPass();
        void directionalLightsPass();
        void forwardPass();

        // Mesh loadMesh();
};

#endif