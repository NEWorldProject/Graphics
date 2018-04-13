//
// Created by dtzx2 on 2018/4/10.
//

#include "Scene.h"
#include <vector>

class PipelineStage0 {
public:
    PipelineStage0() {
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
    void use() { mProg.use(); }
    static auto& instance() {
        static PipelineStage0 ins;
        return ins;
    }
    auto setMVP(const Mat4f mat) {
        glUniformMatrix4fv(mMvp, 1, GL_TRUE, mat.data);
    }
private:
    Program mProg;
    GLint mMvp;
};

Scene::Scene() {
    glGenTextures(1, &mTex0);
    glGenVertexArrays(1, &mVao);
}

Scene::~Scene() { ;
    glDeleteTextures(1, &mTex0);
}

Scene& Scene::instance() {
    static Scene ins;
    return ins;
}

void Scene::setSize(Vec2i size) {
    mRbo.setStorage(GL_STENCIL_INDEX, size);
    for (auto x : { mTex0 }) {
        glBindTexture(GL_TEXTURE_2D, x);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    }
    bindBuffers();
}

void Scene::render(const ISprite& sprite, EFXHandle efx) {
    mFbo.use(GL_FRAMEBUFFER);
    glEnableVertexAttribArray(0);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    // Clear color buffer
    glClear(GL_COLOR_BUFFER_BIT);

    PipelineStage0::instance().use();
    PipelineStage0::instance().setMVP(mTransforms.back());
    glBindVertexArray(mVao);
    mVbo.use(GL_ARRAY_BUFFER);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vert) * sprite.getVertices().size(),
                 sprite.getVertices().data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, nullptr);
    glDrawArrays(GL_TRIANGLES, 0, sprite.getVertices().size());

    glEnable(GL_BLEND);
    glBlendFunc(GL_DST_ALPHA, GL_ZERO);
    sprite.getBrush()->useBrush(mTransforms.back());
    Rect rect = sprite.getRect();
    double rectData[12] = {
            rect.min.x, rect.min.y,
            rect.max.x, rect.min.y,
            rect.min.x, rect.max.y,
            rect.max.x, rect.min.y,
            rect.max.x, rect.max.y,
            rect.min.x, rect.max.y,
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(double) * 12, rectData, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, nullptr);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(0);
    glDisable(GL_BLEND);
}

void Scene::pushTransform(Mat4f mat) {
    mTransforms.push_back(mTransforms.back() * mat);
}

void Scene::popTransform() { mTransforms.pop_back(); }

void Scene::pushClip(const std::vector<Vert>& verts) {}

void Scene::popClip() {}

void Scene::bindBuffers() {
    mFbo.use(GL_FRAMEBUFFER);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTex0, 0);
}

