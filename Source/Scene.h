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
    FrameBuffer mFbo;
private:
    RenderBuffer mRbo;
    DataBuffer mVbo;
    GLuint mTex0;
    GLuint mVao;
    std::vector<Mat4f> mTransforms { Mat4f::identity() };
    void bindBuffers();
};
