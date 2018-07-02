#pragma once

#include <GL/glew.h>
#include <string>
#include <vector>
#include "Core/Utility.h"
#include "Math/Vector.h"

class Shader {
public:
    Shader(GLuint eShaderType, const std::string &strFileData);
    ~Shader() { glDeleteShader(mShader); }
    auto raw() const noexcept { return mShader; }
private:
    GLuint mShader;
};

class Program {
public:
    Program() = default;
    ~Program() { glDeleteProgram(mProgram); }
    void link(const std::vector<Shader*>& shaderList);
    auto raw() const noexcept { return mProgram; }
    void use() {
        glUseProgram(mProgram);
    }
    auto getUniformLoc(const char* name) {
        return glGetUniformLocation(mProgram, name);
    }
private:
    GLuint mProgram;
};

class RenderBuffer {
public:
    RenderBuffer() { glGenRenderbuffers(1, &mRenderBuffer); }
    ~RenderBuffer() { glDeleteRenderbuffers(1, &mRenderBuffer); }
    void setStorage(GLuint fmt, Vec2i size) {
        glNamedRenderbufferStorageEXT(mRenderBuffer, fmt, size.x, size.y);
    }
    auto raw() const noexcept { return mRenderBuffer; }
private:
    GLuint mRenderBuffer{};
};

class FrameBuffer {
public:
    FrameBuffer() { glGenFramebuffers(1, &mFrameBuffer); }
    ~FrameBuffer() { glDeleteFramebuffers(1, &mFrameBuffer); }
    auto raw() const noexcept { return mFrameBuffer; }
    void use(GLuint target){ glBindFramebuffer(target, mFrameBuffer); }
private:
    GLuint mFrameBuffer{};
};

class DataBuffer {
public:
    DataBuffer() { glGenBuffers(1, &mBuffer); }
    ~DataBuffer() { glDeleteBuffers(1, &mBuffer); }
    auto raw() const noexcept { return mBuffer; }
    void use(GLuint target){ glBindBuffer(target, mBuffer); }
    void data(GLsizei size, const void *data, GLenum usage) { glNamedBufferDataEXT(mBuffer, size, data, usage); }
private:
    GLuint mBuffer{};
};

class VertexArray {
public:
    VertexArray() { glGenVertexArrays(1, &mVertexArray); }
    ~VertexArray() { glDeleteVertexArrays(1, &mVertexArray); }
    auto raw() const noexcept { return mVertexArray; }
    void use(){ glBindVertexArray(mVertexArray); }
    void attribFormat(GLuint index, GLuint size, GLenum type, GLboolean normalized, GLuint relativeOffset) {
        glVertexArrayVertexAttribFormatEXT(mVertexArray, index, size, type, normalized, relativeOffset);
    }
    void attribBinding(GLuint attribIndex, GLuint bufferIndex) {
        glVertexArrayVertexAttribBindingEXT(mVertexArray, attribIndex, bufferIndex);
    }
    void bindBuffer(GLuint index, DataBuffer& buffer, GLintptr offset, GLsizei stride) {
        glVertexArrayBindVertexBufferEXT(mVertexArray, index, buffer.raw(), offset, stride);
    }
private:
    GLuint mVertexArray{};
};

class Texture {
public:
    Texture() { glGenTextures(1, &mTexture); }
    ~Texture() { glDeleteTextures(1, &mTexture); }
    auto raw() const noexcept { return mTexture; }
    void use(GLuint target){ glBindTexture(target, mTexture); }
private:
    GLuint mTexture{};
};
