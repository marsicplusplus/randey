#ifndef __RENDERER_HPP__
#define __RENDERER_HPP__

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "core/mesh.hpp"

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

        static void framebufferSizeCB(GLFWwindow *window, int width, int height);
        void processInput();

        Mesh loadMesh();
};

#endif