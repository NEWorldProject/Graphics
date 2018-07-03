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
    void setStorage(GLuint fmt, Vec2i size) { glNamedRenderbufferStorageEXT(mRenderBuffer, fmt, size.x, size.y); }
    auto raw() const noexcept { return mRenderBuffer; }
private:
    GLuint mRenderBuffer{};
};

class DataBuffer {
public:
    DataBuffer() { glGenBuffers(1, &mBuffer); }
    ~DataBuffer() { glDeleteBuffers(1, &mBuffer); }
    auto raw() const noexcept { return mBuffer; }
    void use(GLuint target){ glBindBuffer(target, mBuffer); }
    template <class ContiguousContainer>
    void data(const ContiguousContainer& c, GLenum usage) {
        dataRaw(c.size() * sizeof(ContiguousContainer::value_type), c.data(), usage);
    }
    void dataRaw(GLsizei size, const void *data, GLenum usage) { glNamedBufferDataEXT(mBuffer, size, data, usage); }
private:
    GLuint mBuffer{};
};

class VertexArray {
public:
    VertexArray() { glGenVertexArrays(1, &mVertexArray); }
    ~VertexArray() { glDeleteVertexArrays(1, &mVertexArray); }
    auto raw() const noexcept { return mVertexArray; }
    void use(){ glBindVertexArray(mVertexArray); }
    void enableAttrib(GLuint id) { glEnableVertexArrayAttribEXT(mVertexArray, id); }
    void disableAttrib(GLuint id) { glDisableVertexArrayAttribEXT(mVertexArray, id); }
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
    void image(GLuint level, GLuint internalFormat, Vec2i size, GLuint format, GLuint type, void* data) {
        glTextureImage2DEXT(mTexture, GL_TEXTURE_2D, level, internalFormat, size.x, size.y, 0, format, type, data);
    }
    void setParameter(GLenum name, GLint param) { glTextureParameteriEXT(mTexture, GL_TEXTURE_2D, name, param); }
    void setParameter(GLenum name, GLfloat param) { glTextureParameterfEXT(mTexture, GL_TEXTURE_2D, name, param); }
private:
    GLuint mTexture{};
};

class FrameBuffer {
public:
    FrameBuffer() { glGenFramebuffers(1, &mFrameBuffer); }
    ~FrameBuffer() { glDeleteFramebuffers(1, &mFrameBuffer); }
    auto raw() const noexcept { return mFrameBuffer; }
    void use(GLuint target){ glBindFramebuffer(target, mFrameBuffer); }
    void texture(GLenum attachment, Texture& texture, GLint level) {
        glNamedFramebufferTextureEXT(mFrameBuffer, attachment, texture.raw(), level);
    }
    void renderBuffer(GLenum attachment, GLenum target, RenderBuffer& buffer) {
        glNamedFramebufferRenderbufferEXT(mFrameBuffer, attachment, target, buffer.raw());
    }
private:
    GLuint mFrameBuffer{};
};
