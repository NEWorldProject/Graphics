#pragma once

#include "Core/Utility.h"
#include "Sprite.h"
#include "Effect.h"
#include "GLUtils.h"

class Scene : public NonCopyableVirtualBase {
public:
    Scene();
    ~Scene();
    static Scene& instance();
    void setSize(Vec2i size);
    void render(const ISprite& sprite);
    void setTransform(Mat4f mat);
    void pushClip(const std::vector<Vert>& verts);
    void popClip();
    void prepare();
    FrameBuffer& getResult();
private:
    class RenderPipeline;
    std::unique_ptr<RenderPipeline> mPipeline;
};
