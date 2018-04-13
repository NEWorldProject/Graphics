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
    void render(const ISprite& sprite);
    void pushTransform(Mat4f mat);
    void popTransform();
    void pushClip(const std::vector<Vert>& verts);
    void popClip();
    FrameBuffer& getResult();
private:
    class RenderPipeline;
    std::unique_ptr<RenderPipeline> mPipeline;
};
