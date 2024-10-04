#ifndef __RENDERER_HPP__
#define __RENDERER_HPP__

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "core/camera.hpp"
#include "core/mesh.hpp"
#include "core/model.hpp"
#include "core/cubemap_texture.hpp"
#include "lights/point_light.hpp"
#include "lights/directional_light.hpp"
#include "buffers/gbuffer.hpp"
#include "buffers/shadow_map_FBO.hpp"

#include <memory>

class Renderer {
    public:
        Renderer(uint32_t width, uint32_t height);
        ~Renderer();

        bool init(const std::string &filePath);
        bool start();
        
    private:
        // Window:
        GLFWwindow *mWindow;
        uint32_t mWidth, mHeight;

        // Scene:
        Camera* mCamera;
        std::vector<ModelPtr> mModels;
        std::shared_ptr<SphereMesh> mSphereMesh;
        std::shared_ptr<CubeMesh> mCubeMesh;
        std::vector<PointLightPtr> mPointLights;
        std::vector<DirectionalLightPtr> mDirLights;

        // Techniques:
        ShaderPtr mGeometryShader;
        ShaderPtr mPointLightsShader;
        ShaderPtr mDirectionalLightsShader;
        ShaderPtr mLightRenderingShader;
        ShaderPtr mStencilPassShader;
        ShaderPtr mDirectionalShadowMapShader;
        ShaderPtr mPointShadowMapShader;
        ShaderPtr mSkyboxShader;
        ShaderPtr mTransparencyShader;

        // Buffers & Textures:
        GBuffer mGBuffer;
        std::shared_ptr<CubemapTexture> mSkyboxTexture;

        // Matrices:
        glm::mat4 mProjection;

        // Quad
        uint32_t mQuadVAO;
        uint32_t mQuadVBO;

        // Settings:
        bool mDrawLights = true;

        static void framebufferSizeCB(GLFWwindow *window, int width, int height);
        void processInput();

        // Techniques passes:
        void geometryPass();
        void lightPass();
        void pointLightsPass();
        void directionalLightsPass();
        void drawPointLights();
        void shadowMapPass();
        void transparencyPass();
        void skyboxPass();

        // Mesh loadMesh();
};

#endif