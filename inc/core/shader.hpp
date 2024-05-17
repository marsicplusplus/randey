#ifndef __SHADER_HPP__
#define __SHADER_HPP__

#include <glad/glad.h>

#include <string>
#include <map>

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

        uint32_t mID;

    private:
        std::map<uint32_t, uint32_t> mShaders;
};

#endif