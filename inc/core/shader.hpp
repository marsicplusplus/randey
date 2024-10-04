#ifndef __SHADER_HPP__
#define __SHADER_HPP__

#include <glad/glad.h>

#include <string>
#include <map>
#include <memory>

#include "glm/mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"

class Shader {
    public:
        Shader();
        ~Shader();
        void attachShader(const char* path, uint32_t shaderType);
        void use();
        void link();

        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;
        void setMat4(const std::string &name, const glm::mat4 &value) const;
        void setVec2(const std::string &name, const glm::vec2 &value) const;
        void setVec2(const std::string &name, float x, float y) const;
        void setVec3(const std::string &name, const glm::vec3 &value) const;
        void setVec3(const std::string &name, float x, float y, float z) const;

        uint32_t mID;

    private:
        std::map<uint32_t, uint32_t> mShaders;
};

typedef std::shared_ptr<Shader> ShaderPtr;

#endif