#pragma once

#include <GL/glew.h>
#include <string>
#include <vector>
#include "Math/Vector.h"
#include "Rect.h"

class GLHandle {
public:
    auto raw() const noexcept { return mHdc; }
protected:
    GLuint mHdc{};
};

class Shader : public GLHandle {
public:
    Shader(GLuint eShaderType, const std::string &strFileData);
    ~Shader() noexcept;
};

class Program : public GLHandle {
public:
    Program() noexcept = default;
    ~Program() noexcept;
    void link(const std::vector<Shader*>& shaderList);
    void use() noexcept;
    auto getUniformLoc(const char* name) noexcept;
    void bindUniformBlock(const char* name, GLuint index) noexcept;
};

class RenderBuffer : public GLHandle {
public:
    RenderBuffer() noexcept;
    ~RenderBuffer() noexcept;
    void setStorage(GLuint fmt, Vec2i size) noexcept;
};

class DataBuffer : public GLHandle {
public:
    DataBuffer() noexcept;
    ~DataBuffer() noexcept;
    template <class ContiguousContainer>
    void data(const ContiguousContainer& c, GLenum usage) noexcept {
        dataRaw(static_cast<GLsizei>(c.size() * sizeof(ContiguousContainer::value_type)), c.data(), usage);
    }
    template <class ContiguousContainer>
    void dataSection(GLintptr offset, const ContiguousContainer& c) noexcept {
        dataSectionRaw(offset, static_cast<GLsizei>(c.size() * sizeof(ContiguousContainer::value_type)), c.data());
    }
    void dataRaw(GLsizei size, const void *data, GLenum usage) noexcept;
    void dataSectionRaw(GLintptr offset, GLsizei size, const void *data);
    void bindBase(GLenum usage, GLuint index) noexcept;
};

class VertexArray : public GLHandle {
public:
    VertexArray() noexcept;
    ~VertexArray() noexcept;
    void use() noexcept;
    void enableAttrib(GLuint id) noexcept;
    void disableAttrib(GLuint id) noexcept;
    void attribFormat(GLuint index, GLuint size, GLenum type, bool normalized, GLuint relativeOffset) noexcept;
    void attribBinding(GLuint attribIndex, GLuint bufferIndex) noexcept;
    void bindBuffer(GLuint index, const DataBuffer &buffer, GLintptr offset, GLsizei stride) noexcept;
};

class Texture : public GLHandle {
public:
    Texture() noexcept;
    ~Texture() noexcept;
    void use(GLuint target) noexcept;
    void image(GLuint level, GLuint internalFormat, Vec2i size, GLuint format, GLuint type, void* data) noexcept;
    void setParameter(GLenum name, GLint param) noexcept;
    void setParameter(GLenum name, GLfloat param) noexcept;
};

class FrameBuffer : public GLHandle {
public:
    enum BufferType : unsigned int {
        BufferColor, BufferDepth, BufferStencil
    };
    FrameBuffer() noexcept;
    ~FrameBuffer() noexcept;
    void use(GLuint target) noexcept;
    void texture(GLenum attachment, Texture& texture, GLint level) noexcept;
    void renderBuffer(GLenum attachment, GLenum target, RenderBuffer& buffer) noexcept;
};

void blitFrameBuffers(FrameBuffer& dst, const FrameBuffer& src, const Rect& to, const Rect& from);
