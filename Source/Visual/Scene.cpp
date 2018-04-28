//
// Created by dtzx2 on 2018/4/10.
//

#include "Scene.h"

namespace {
    class Stage {
    public:
        void resize(Vec2i size) {
            mTex.use(GL_TEXTURE_2D);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
            mFbo.use(GL_FRAMEBUFFER);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTex.get(), 0);
        }

        FrameBuffer &getResult() { return mFbo; }

        Texture& getPipelineResult() { return mTex; }

        void useFbo() { mFbo.use(GL_FRAMEBUFFER); }

    protected:
        Texture mTex;
        FrameBuffer mFbo;
    };

}

class Scene::RenderPipeline final {
public:
    void resize(Vec2i size);

    void render(const ISprite& sprite);

    void pushTransform(const Mat4f &mat) {
        mUMvp.use(GL_UNIFORM_BUFFER);
        auto transposed = mat.getTranspose();
        glBufferData(GL_UNIFORM_BUFFER, sizeof(Mat4f), transposed.data, GL_DYNAMIC_DRAW);
    }

    FrameBuffer& getResult() { return mStage1.getResult(); }

    void prepare();
private:

    void blenderBrush(const ISprite& sprite);

    void applyEFX(const ISprite& sprite);

    void commonBrush(const ISprite& sprite);

    Effect* getEFX(const ISprite& sprite) {
        return sprite.getEFX() ? sprite.getEFX().get() : Effect::getDefault().get();
    }

    Brush* getBrush(const ISprite& sprite) {
        return sprite.getBrush() ? sprite.getBrush().get() : Brush::getDefault().get();
    }

    Stage mStage0;
    Stage mStage1;
    DataBuffer mVbo;
    VertexArray mVao;
    RenderBuffer mStencil;
    DataBuffer mUMvp, mURect, mUResult;
};

void Scene::RenderPipeline::resize(Vec2i size) {
    mStage0.resize(size);
    mStage1.resize(size);
    mStencil.setStorage(GL_STENCIL_INDEX, size);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_BUFFER, GL_RENDERBUFFER, mStencil.get());
}

void Scene::RenderPipeline::render(const ISprite& sprite) {
    if (!getBrush(sprite)->isBlender() && !sprite.getEFX()) {
        commonBrush(sprite);
    }
    else {
        mStage0.useFbo();
        blenderBrush(sprite);
        mStage1.useFbo();
        applyEFX(sprite);
    }
}

void Scene::RenderPipeline::prepare() {
    mStage1.useFbo();
    mVao.use();
    glEnableVertexAttribArray(0);
    mVbo.use(GL_ARRAY_BUFFER);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, nullptr);
    mUResult.use(GL_UNIFORM_BUFFER);
    GLuint resData = GL_TEXTURE_2D;
    glBufferData(GL_UNIFORM_BUFFER, sizeof(GLuint), &resData, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, mUMvp.get());
    glBindBufferBase(GL_UNIFORM_BUFFER, 3, mUResult.get());
}

void Scene::RenderPipeline::blenderBrush(const ISprite& sprite) {
    glDisable(GL_BLEND);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    commonBrush(sprite);
}

void Scene::RenderPipeline::applyEFX(const ISprite& sprite) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    auto rect = sprite.getRect();
    getEFX(sprite)->use();
    const double rectData[12] = {
            rect.min.x, rect.min.y, rect.max.x, rect.min.y, rect.min.x, rect.max.y,
            rect.max.x, rect.min.y, rect.max.x, rect.max.y, rect.min.x, rect.max.y,
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectData), rectData, GL_STREAM_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Scene::RenderPipeline::commonBrush(const ISprite& sprite) {
    getBrush(sprite)->useBrush();
    auto [data, size] = sprite.getVertices();
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vert) * size, data, GL_STREAM_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, size);
}

Scene::Scene() : mPipeline(std::make_unique<RenderPipeline>()) {
    BrushType::coInitialize();
}

Scene::~Scene() = default;

Scene& Scene::instance() {
    static Scene ins;
    return ins;
}

void Scene::setSize(Vec2i size) { mPipeline->resize(size); }

void Scene::setTransform(Mat4f mat) { mPipeline->pushTransform(mat); }

void Scene::pushClip(const std::vector<Vert>& verts) {}

void Scene::popClip() {}

void Scene::render(const ISprite &sprite) { mPipeline->render(sprite); }

FrameBuffer &Scene::getResult() {
    return mPipeline->getResult();
}

void Scene::prepare() { mPipeline->prepare(); }
