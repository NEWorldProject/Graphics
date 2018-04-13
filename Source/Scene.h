#pragma once

#include "Utility.h"
#include "Sprite.h"
#include "EFX.h"
#include "GLUtils.h"

class Scene : public NonCopyable {
public:
    Scene();
    ~Scene();
    static Scene& instance();
    void setSize(Vec2i size);
    void render(const ISprite& sprite, EFXHandle efx);
    void pushTransform(Mat4f mat);
    void popTransform();
    void pushClip(const std::vector<Vert>& verts);
    void popClip();
    GLuint mTex0;
    GLuint mFbo;
    GLuint mRbo; // For Stencil Buffers
    GLuint mVbo, mVao;
    std::vector<Mat4f> mTransforms { Mat4f::identity() };
private:
    void bindBuffers();
    void unbindBuffers();
};
