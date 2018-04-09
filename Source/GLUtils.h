#pragma once

#include <GL/glew.h>
#include <string>
#include <vector>
#include "Utility.h"

class Shader: public NonCopyable {
public:
    Shader(GLuint eShaderType, const std::string &strFileData) {
        GLuint shader = glCreateShader(eShaderType);
        auto file = strFileData.c_str();
        glShaderSource(shader, 1, &file, nullptr);
        glCompileShader(shader);
        GLint status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE)
        {
            GLint infoLogLength;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
            GLchar *strInfoLog = new GLchar[infoLogLength + 1];
            glGetShaderInfoLog(shader, infoLogLength, nullptr, strInfoLog);
            const char * strShaderType = nullptr;
            switch (eShaderType)
            {
                case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
                case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
                case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
            }
            fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
            delete[] strInfoLog;
        }
    }
    auto get() const noexcept { return mShader; }
private:
    GLuint mShader;
};

class Program: public NonCopyable {
public:
    Program() = default;
    void link(const std::vector<Shader*>& shaderList) {
        GLuint programId = glCreateProgram();
        for (auto&& sd : shaderList)
            glAttachShader(programId, sd->get());
        glLinkProgram(programId);
        GLint status;
        glGetProgramiv(programId, GL_LINK_STATUS, &status);
        if (status == GL_FALSE)
        {
            GLint infoLogLength;
            glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);

            GLchar *strInfoLog = new GLchar[infoLogLength + 1];
            glGetProgramInfoLog(programId, infoLogLength, nullptr, strInfoLog);
            fprintf(stderr, "Linker failure: %s\n", strInfoLog);
            delete[] strInfoLog;
        }
        mProgram = programId;
    }
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
