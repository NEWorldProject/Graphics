#include "Graphics/GLUtils.h"

Shader::Shader(GLuint eShaderType, const std::string &strFileData) {
    GLuint shader = glCreateShader(eShaderType);
    auto file = strFileData.c_str();
    glShaderSource(shader, 1, &file, nullptr);
    glCompileShader(shader);
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        GLint infoLogLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
        std::vector<GLchar> strInfoLog(infoLogLength + 1);
        glGetShaderInfoLog(shader, infoLogLength, nullptr, strInfoLog.data());
        const char *strShaderType = nullptr;
        switch (eShaderType) {
            case GL_VERTEX_SHADER:
                strShaderType = "vertex";
                break;
            case GL_GEOMETRY_SHADER:
                strShaderType = "geometry";
                break;
            case GL_FRAGMENT_SHADER:
                strShaderType = "fragment";
                break;
            default:
                break;
        }
        throw std::runtime_error(std::string("Compile failure in ") + strShaderType + " shader:" + strInfoLog.data());
    }
    mHdc = shader;
}

Shader::~Shader() noexcept { glDeleteShader(mHdc); }

void Program::link(const std::vector<Shader*> &shaderList) {
    GLuint programId = glCreateProgram();
    for (auto &&sd : shaderList)
        glAttachShader(programId, sd->raw());
    glLinkProgram(programId);
    GLint status;
    glGetProgramiv(programId, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        GLint infoLogLength;
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);
        std::vector<GLchar> strInfoLog(infoLogLength + 1);
        glGetProgramInfoLog(programId, infoLogLength, nullptr, strInfoLog.data());
        throw std::runtime_error(std::string("Linker failure: ") + strInfoLog.data());
    }
    mHdc = programId;
}

void Program::use() noexcept { glUseProgram(mHdc); }

auto Program::getUniformLoc(const char *name) noexcept { return glGetUniformLocation(mHdc, name); }

Program::~Program() noexcept { glDeleteProgram(mHdc); }

void Program::bindUniformBlock(const char *name, GLuint index) noexcept {
    GLuint blockIndex = glGetUniformBlockIndex(mHdc, name);
    glUniformBlockBinding(mHdc, blockIndex, index);
}

RenderBuffer::RenderBuffer() noexcept { glGenRenderbuffers(1, &mHdc); }

RenderBuffer::~RenderBuffer() noexcept { glDeleteRenderbuffers(1, &mHdc); }

void RenderBuffer::setStorage(GLuint fmt, Vec2i size) noexcept {
    glNamedRenderbufferStorageEXT(mHdc, fmt, size.x, size.y);
}

DataBuffer::DataBuffer() noexcept { glGenBuffers(1, &mHdc); }

DataBuffer::~DataBuffer() noexcept { glDeleteBuffers(1, &mHdc); }

void DataBuffer::dataRaw(GLsizei size, const void *data, GLenum usage) noexcept {
    glNamedBufferDataEXT(mHdc, size, data, usage);
}

void DataBuffer::dataSectionRaw(GLintptr offset, GLsizei size, const void *data) {
    glNamedBufferSubDataEXT(mHdc, offset, size, data);
}

void DataBuffer::bindBase(GLenum usage, GLuint index) noexcept { glBindBufferBase(usage, index, mHdc); }

VertexArray::VertexArray() noexcept { glGenVertexArrays(1, &mHdc); }

VertexArray::~VertexArray() noexcept { glDeleteVertexArrays(1, &mHdc); }

void VertexArray::use() noexcept { glBindVertexArray(mHdc); }

void VertexArray::enableAttrib(GLuint id) noexcept { glEnableVertexArrayAttribEXT(mHdc, id); }

void VertexArray::disableAttrib(GLuint id) noexcept { glDisableVertexArrayAttribEXT(mHdc, id); }

void VertexArray::attribFormat(GLuint index, GLuint size, GLenum type, bool normalized,
                               GLuint relativeOffset)  noexcept {
    use(); //work around for Intel driver version 21.*
    glVertexArrayVertexAttribFormatEXT(mHdc, index, size, type, static_cast<GLboolean>(normalized), relativeOffset);
}

void VertexArray::attribBinding(GLuint attribIndex, GLuint bufferIndex) noexcept {
    glVertexArrayVertexAttribBindingEXT(mHdc, attribIndex, bufferIndex);
}

void VertexArray::bindBuffer(GLuint index, const DataBuffer &buffer, GLintptr offset, GLsizei stride) noexcept {
    glVertexArrayBindVertexBufferEXT(mHdc, index, buffer.raw(), offset, stride);
}

void Texture::image(GLuint level, GLuint internalFormat, Vec2i size, GLuint format, GLuint type, void *data) noexcept {
    glTextureImage2DEXT(mHdc, GL_TEXTURE_2D, level, internalFormat, size.x, size.y, 0, format, type, data);
}

void Texture::setParameter(GLenum name, GLint param) noexcept {
    glTextureParameteriEXT(mHdc, GL_TEXTURE_2D, name, param);
}

void Texture::setParameter(GLenum name, GLfloat param) noexcept {
    glTextureParameterfEXT(mHdc, GL_TEXTURE_2D, name, param);
}

Texture::Texture() noexcept { glGenTextures(1, &mHdc); }

Texture::~Texture() noexcept { glDeleteTextures(1, &mHdc); }

void Texture::use(GLuint target) noexcept { glBindTexture(target, mHdc); }

FrameBuffer::FrameBuffer() noexcept { glGenFramebuffers(1, &mHdc); }

FrameBuffer::~FrameBuffer() noexcept { glDeleteFramebuffers(1, &mHdc); }

void FrameBuffer::use(GLuint target) noexcept { glBindFramebuffer(target, mHdc); }

void FrameBuffer::texture(GLenum attachment, Texture &texture, GLint level) noexcept {
    glNamedFramebufferTextureEXT(mHdc, attachment, texture.raw(), level);
}

void FrameBuffer::renderBuffer(GLenum attachment, GLenum target, RenderBuffer &buffer) noexcept {
    glNamedFramebufferRenderbufferEXT(mHdc, attachment, target, buffer.raw());
}

void blitFrameBuffers(FrameBuffer &dst, const FrameBuffer &src, const Rect &to, const Rect &from) {
}
