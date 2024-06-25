#ifndef __RENDERER_HPP__
#define __RENDERER_HPP__

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "core/camera.hpp"
#include "core/mesh.hpp"
#include "buffers/gbuffer.hpp"

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
        std::vector<Mesh> mMeshes;
        ShaderPtr mGeometryShader;
        GBuffer mGBuffer;
        glm::mat4 mProjection;

        static void framebufferSizeCB(GLFWwindow *window, int width, int height);
        void processInput();
        void geometryPass();
        void lightPass();

        // Mesh loadMesh();
};

#endif