#pragma once

#include <GL/glew.h>
#include <string>
#include <vector>
#include "Utility.h"

class Shader: public NonCopyable {
public:
    Shader(GLuint eShaderType, const std::string &strFileData);
    auto get() const noexcept { return mShader; }
private:
    GLuint mShader;
};

class Program: public NonCopyable {
public:
    Program() = default;
    void link(const std::vector<Shader*>& shaderList);
    auto get() const noexcept { return mProgram; }
    void use() {
        glUseProgram(mProgram);
    }
    auto getUniformLoc(const std::string& name) {
        return glGetUniformLocation(mProgram, name.c_str());
    }
private:
    GLuint mProgram;
};
