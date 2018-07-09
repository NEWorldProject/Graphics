#pragma once

#include <string>
#include <vector>
#include <atomic>
#include <thread>
#include <GL/glew.h>
#include <functional>
#include "Rect.h"
#include "Math/Vector.h"
#include "GIL/RGBA.h"

namespace Graphics {
    class Window;

    class Context {
    public:
        Context() = default;
        explicit Context(Window &window);
        Context(Context &&rhs) noexcept;
        Context &operator=(Context &&rhs) noexcept;
        Context(const Context &) = delete;
        Context &operator=(const Context &) = delete;
        ~Context() noexcept;
        void unBindCurrent() noexcept;
        void makeCurrent() noexcept;
        void startRenderThread(std::function<void()> renderFunction);
        void closeRenderThread() noexcept;
        void *raw() const noexcept { return mHdc; }
    private:
        void *mHdc = nullptr, *mHost = nullptr;
        std::atomic_bool mRenderFlag{false};
        std::thread mRenderThread;
    };

    class GLHandle {
    public:
        GLHandle() noexcept = default;
        GLHandle(GLHandle&&) = delete;
        GLHandle(const GLHandle&) = delete;
        GLHandle& operator = (GLHandle&&) = delete;
        GLHandle& operator = (const GLHandle&&) = delete;
        auto raw() const noexcept { return mHdc; }
    protected:
        GLuint mHdc{0};
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
        void link(const std::vector<Shader *> &shaderList);
        void use() noexcept;
        auto getUniformLoc(const char *name) noexcept;
        void bindUniformBlock(const char *name, GLuint index) noexcept;
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

        template<class ContiguousContainer>
        void data(const ContiguousContainer &c, GLenum usage) noexcept {
            dataRaw(static_cast<GLsizei>(c.size() * sizeof(ContiguousContainer::value_type)), c.data(), usage);
        }

        template<class ContiguousContainer>
        void dataSection(GLintptr offset, const ContiguousContainer &c) noexcept {
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
        static VertexArray &getDefault() noexcept;
    };

    class Texture : public GLHandle {
    public:
        Texture() noexcept;
        ~Texture() noexcept;
        void use(GLuint target) noexcept;
        void image(GLuint level, GLuint internalFormat, Vec2i size, GLuint format, GLuint type, void *data) noexcept;
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
        void clearColor(GLuint buffer, const Graphics::RGBA::f32::Color &color) noexcept;
        void texture(GLenum attachment, Texture &texture, GLint level) noexcept;
        void renderBuffer(GLenum attachment, GLenum target, RenderBuffer &buffer) noexcept;
        static FrameBuffer &getDefault() noexcept;
    };

    void blitFrameBuffers(FrameBuffer &dst, const FrameBuffer &src, const Rect &to, const Rect &from);
}
