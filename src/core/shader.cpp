#include "core/shader.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader() {
    mShaders[GL_FRAGMENT_SHADER] = 0;
    mShaders[GL_GEOMETRY_SHADER] = 0;
    mShaders[GL_COMPUTE_SHADER] = 0;
    mShaders[GL_VERTEX_SHADER] = 0;
}

void Shader::attachShader(const char* path, uint32_t shaderType) {
    std::string code;
    std::ifstream shaderFile;
    // ensure ifstream objects can throw exceptions:
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        shaderFile.open(path);
        std::stringstream shaderStream;
        // read file's buffer contents into streams
        shaderStream << shaderFile.rdbuf();
        // close file handlers
        shaderFile.close();
        // convert stream into string
        code = shaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char *shaderCode = code.c_str();

    unsigned int shaderId;
    int success;
    char infoLog[512];

    shaderId = glCreateShader(shaderType);
    glShaderSource(shaderId, 1, &shaderCode, NULL);
    glCompileShader(shaderId);
    // print compile errors if any
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    };

    mShaders[shaderType] = shaderId;
}

void Shader::link() {
    mID = glCreateProgram();
    if(mShaders[GL_VERTEX_SHADER] != 0) {
        glAttachShader(mID, mShaders[GL_VERTEX_SHADER]);
    }
    if(mShaders[GL_FRAGMENT_SHADER] != 0) {
        glAttachShader(mID, mShaders[GL_FRAGMENT_SHADER]);
    }
    if(mShaders[GL_GEOMETRY_SHADER] != 0) {
        glAttachShader(mID, mShaders[GL_GEOMETRY_SHADER]);
    }
    if(mShaders[GL_COMPUTE_SHADER] != 0) {
        glAttachShader(mID, mShaders[GL_COMPUTE_SHADER]);
    }
    int success;
    char infoLog[512];
    glLinkProgram(mID);
    // print linking errors if any
    glGetProgramiv(mID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(mID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }

    if (mShaders[GL_FRAGMENT_SHADER] != 0) {
        glDetachShader(mID, mShaders[GL_VERTEX_SHADER]);
        glDeleteShader(mShaders[GL_VERTEX_SHADER]);
    }
    if (mShaders[GL_FRAGMENT_SHADER] != 0) {
        glDetachShader(mID, mShaders[GL_FRAGMENT_SHADER]);
        glDeleteShader(mShaders[GL_FRAGMENT_SHADER]);
    }
    if (mShaders[GL_COMPUTE_SHADER] != 0) {
        glDetachShader(mID, mShaders[GL_COMPUTE_SHADER]);
        glDeleteShader(mShaders[GL_COMPUTE_SHADER]);
    }
    if (mShaders[GL_GEOMETRY_SHADER] != 0) {
        glDetachShader(mID, mShaders[GL_GEOMETRY_SHADER]);
        glDeleteShader(mShaders[GL_GEOMETRY_SHADER]);
    }
}

void Shader::use() {
    glUseProgram(mID);
}


void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(mID, name.c_str()), (int)value); 
}
void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(mID, name.c_str()), value); 
}
void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(mID, name.c_str()), value); 
} 
void Shader::setMat4(const std::string &name, const glm::mat4 &value) const {
    glUniformMatrix4fv(glGetUniformLocation(mID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value)); 
}
void Shader::setVec3(const std::string &name, const glm::vec3 &value) const {
    glUniform3f(glGetUniformLocation(mID, name.c_str()), value.x, value.y, value.z); 
}
void Shader::setVec3(const std::string &name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(mID, name.c_str()), x, y, z); 
}
void Shader::setVec2(const std::string &name, const glm::vec2 &value) const {
    glUniform2f(glGetUniformLocation(mID, name.c_str()), value.x, value.y); 
}
void Shader::setVec2(const std::string &name, float x, float y) const {
    glUniform2f(glGetUniformLocation(mID, name.c_str()), x, y); 
}

Shader::~Shader() {
    mShaders.clear();
}