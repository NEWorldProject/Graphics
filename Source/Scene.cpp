//
// Created by dtzx2 on 2018/4/10.
//

#include "Scene.h"
#include <vector>

namespace {
    class ShaderStage0 final {
    public:
        ShaderStage0() {
            Shader vert { GL_VERTEX_SHADER, R"(
#version 330
in vec2 pos;
uniform mat4 mvp;
void main() {
   gl_Position = mvp * vec4(pos.x, pos.y, 0.0, 1.0);
}
)"
            };
            Shader frag{ GL_FRAGMENT_SHADER, R"(
#version 330
out vec4 outputColor;
void main() {
    outputColor = vec4(0.0, 0.0, 0.0, 1.0);
}
)"
            };
            mProg.link({ &vert, &frag });
            mMvp = mProg.getUniformLoc("mvp");
        }
        void use(const Mat4f mat) {
            mProg.use();
            glUniformMatrix4fv(mMvp, 1, GL_TRUE, mat.data);
        }
    private:
        Program mProg;
        GLint mMvp;
    };

    class Stage0 final {
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

        void render(const ISprite& sprite, const Mat4f& transform) {
            setup();
            clear();
            mask(sprite, transform);
            brush(sprite, transform);
            cleanUp();
        }

        FrameBuffer &getResult() {
            return mFbo;
        }

    private:
        void setup() {
            glDisable(GL_BLEND);
            mFbo.use(GL_FRAMEBUFFER);
            glEnableVertexAttribArray(0);
            mVao.use();
            mVbo.use(GL_ARRAY_BUFFER);
        }

        void clear() {
            glClearColor(0.0, 0.0, 0.0, 0.0);
            glClear(GL_COLOR_BUFFER_BIT);
        }

        void mask(const ISprite& sprite, const Mat4f& transform) {
            mShaderStage0.use(transform);
            glBufferData(GL_ARRAY_BUFFER, sizeof(Vert) * sprite.getVertices().size(),
                         sprite.getVertices().data(), GL_STREAM_DRAW);
            glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, nullptr);
            glDrawArrays(GL_TRIANGLES, 0, sprite.getVertices().size());
        }

        void brush(const ISprite& sprite, const Mat4f& transform) {
            glEnable(GL_BLEND);
            glBlendFunc(GL_DST_ALPHA, GL_ZERO);
            sprite.getBrush()->useBrush(transform);
            Rect rect = sprite.getRect();
            const double rectData[12] = {
                    rect.min.x, rect.min.y, rect.max.x, rect.min.y, rect.min.x, rect.max.y,
                    rect.max.x, rect.min.y, rect.max.x, rect.max.y, rect.min.x, rect.max.y,
            };
            glBufferData(GL_ARRAY_BUFFER, sizeof(rectData), rectData, GL_STREAM_DRAW);
            glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, nullptr);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glDisable(GL_BLEND);
        }

        void cleanUp() {
            glDisableVertexAttribArray(0);
        }

        Texture mTex;
        DataBuffer mVbo;
        VertexArray mVao;
        FrameBuffer mFbo;
        ShaderStage0 mShaderStage0;
    };
}

class Scene::RenderPipeline final {
public:
    void resize(Vec2i size) {
        mStage0.resize(size);
    }
    void render(const ISprite& sprite) {
        mStage0.render(sprite, mTransforms.back());
    }

    void pushTransform(Mat4f mat) {
        mTransforms.push_back(mTransforms.back() * mat);
    }

    void popTransform() { mTransforms.pop_back(); }

    FrameBuffer& getResult() { return mStage0.getResult(); }
private:
    Stage0 mStage0;
    std::vector<Mat4f> mTransforms { Mat4f::identity() };
};

Scene::Scene() : mPipeline(std::make_unique<RenderPipeline>()) {}

Scene::~Scene() = default;

Scene& Scene::instance() {
    static Scene ins;
    return ins;
}

void Scene::setSize(Vec2i size) { mPipeline->resize(size); }

void Scene::pushTransform(Mat4f mat) { mPipeline->pushTransform(mat); }

void Scene::popTransform() { mPipeline->popTransform(); }

void Scene::pushClip(const std::vector<Vert>& verts) {}

void Scene::popClip() {}

void Scene::render(const ISprite &sprite) { mPipeline->render(sprite); }

FrameBuffer &Scene::getResult() {
    return mPipeline->getResult();
}
