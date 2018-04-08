#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include "Math/Matrix.h"
#include <GL/glew.h>
#include <SDL.h>

struct NonCopyable {
    NonCopyable() = default;
    NonCopyable(NonCopyable&&) = default;
    NonCopyable& operator = (NonCopyable&&) = default;
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator = (const NonCopyable&)= delete;
    ~NonCopyable() = default;
};

struct Rect {
    Vec2i min, max;
};

using Vert = Vec3d;

class Brush: public NonCopyable {

};

using BrushHandle = std::shared_ptr<Brush>;

class EFX: public NonCopyable {

};

using EFXHandle = std::shared_ptr<EFX>;

class Sprite: public NonCopyable {
public:
    Sprite() = default;
    ~Sprite() = default;
    void setRect(const Rect& rect) noexcept { mRect = rect; }
    auto getRect() const noexcept { return mRect; }
    void setBrush(BrushHandle brush) { mBrush = std::move(brush); }
    auto getBrush() const { return mBrush; }
    template <class T>
    void setVertices(const T<Vert>& in){ std::copy(in.begin(), in.end(), mVertices.begin()); }
    auto& getVertices() const noexcept { return mVertices; }
private:
    Rect mRect;
    std::vector<Vert> mVertices;
    BrushHandle mBrush;
};

class Scene : public NonCopyable {
public:
    Scene() {
        glGenFramebuffers(3, mFBO);
        glGenRenderbuffers(2, mRBO);
        glGenTextures(1, &mTex0);
        glGenTextures(1, &mTexOut);
        glGenTextures(1, &mTexFront);
        glBindFramebuffer(GL_FRAMEBUFFER, mFBO[0]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, mTex0, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRBO[0]);
        glBindFramebuffer(GL_FRAMEBUFFER, mFBO[1]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, mTexOut, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRBO[1]);
        glBindFramebuffer(GL_FRAMEBUFFER, mFBO[2]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, mTexFront, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    ~Scene() {
        glDeleteFramebuffers(3, mFBO);
        glDeleteRenderbuffers(2, mRBO);
        glDeleteTextures(1, &mTex0);
        glDeleteTextures(1, &mTexOut);
        glDeleteTextures(1, &mTexFront);
    }
    void setSize(Vec2i size) {
        for (auto x : { mRBO[0], mRBO[1] }) {
            glBindRenderbuffer(GL_RENDERBUFFER, x);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_INTERNALFORMAT_STENCIL_TYPE, size.x, size.y);
        }
        for (auto x : { mTex0, mTexOut, mTexFront }) {
            glBindTexture(GL_TEXTURE_2D, x);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        }
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    void render(const Sprite& sprite, EFXHandle efx) {

    }
    void pushTransform(Mat4d mat);
    void popTransform();
    void pushClip(const std::vector<Vert>&);
private:
    GLuint mTex0;
    GLuint mTexOut;
    GLuint mTexFront;
    GLuint mFBO[3];
    GLuint mRBO[2]; // For Stencil Buffers
};

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
