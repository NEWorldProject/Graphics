#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include "Math/Matrix.h"
#include <SDL.h>
#include "GLUtils.h"
#include "Utility.h"

struct Rect {
    Vec2i min, max;
};

using Vert = Vec2d;

class Brush: public NonCopyable {
public:
    virtual void useBrush() = 0;
    virtual ~Brush() {}
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
    void setVertices(const std::vector<Vert>& in) { mVertices = in; }
    auto& getVertices() const noexcept { return mVertices; }
private:
    Rect mRect;
    std::vector<Vert> mVertices;
    BrushHandle mBrush;
};

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
        Shader frag{GL_VERTEX_SHADER, R"(
#version 330
out vec4 outputColor;
void main() {
    outputColor = vec4(0.0, 0.0, 0.0, 0.0);
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
    auto setMVP(const Mat4d& mat) {
        glUniformMatrix4dv(mMvp, 1, GL_FALSE, mat.data);
    }
private:
    Program mProg;
    GLint mMvp;
};

class Scene : public NonCopyable {
public:
    Scene() {
        glGenFramebuffers(3, mFbo);
        glGenRenderbuffers(2, mRbo);
        glGenTextures(1, &mTex0);
        glGenTextures(1, &mTexOut);
        glGenTextures(1, &mTexFront);
        glBindFramebuffer(GL_FRAMEBUFFER, mFbo[0]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, mTex0, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRbo[0]);
        glBindFramebuffer(GL_FRAMEBUFFER, mFbo[1]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, mTexOut, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRbo[1]);
        glBindFramebuffer(GL_FRAMEBUFFER, mFbo[2]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, mTexFront, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glGenVertexArrays(1, &mVao);
        glGenBuffers(1, &mVbo);
    }
    ~Scene() {
        glDeleteFramebuffers(3, mFbo);
        glDeleteRenderbuffers(2, mRbo);
        glDeleteTextures(1, &mTex0);
        glDeleteTextures(1, &mTexOut);
        glDeleteTextures(1, &mTexFront);
    }
    void setSize(Vec2i size) {
        for (auto x : { mRbo[0], mRbo[1] }) {
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
        PipelineStage0::instance().use();
        PipelineStage0::instance().setMVP(mTransforms.back());
        glBindFramebuffer(GL_FRAMEBUFFER, mFbo[0]);
        glEnable(GL_STENCIL_TEST);
        glEnable(GL_BLEND);
        glClearStencil(0);
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glStencilFunc(GL_ALWAYS, 1, 1);
        glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
        glBindVertexArray(mVao);
        glBindBuffer(GL_ARRAY_BUFFER, mVbo);
        glEnableVertexAttribArray(0);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vert) * sprite.getVertices().size(),
                sprite.getVertices().data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, nullptr);
        glDrawArrays(GL_TRIANGLES, 0, sprite.getVertices().size());
        glStencilFunc(GL_EQUAL, 1, 1);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        sprite.getBrush()->useBrush();
        Rect rect = sprite.getRect();
        double rect[12] = {
                rect.min.x, rect.min.y,
                rect.max.x, rect.min.y,
                rect.min.x, rect.max.y,
                rect.max.x, rect.min.y,
                rect.max.x, rect.max.y,
                rect.min.x, rect.max.y,
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(double) * 6, sprite.getVertices().data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, nullptr);
        glDrawArrays(GL_TRIANGLES, 0, sprite.getVertices().size());
        glDisableVertexAttribArray(0);

    }
    void pushTransform(Mat4d mat) {
        mTransforms.push_back(mTransforms.back() * mat);
    }
    void popTransform() { mTransforms.pop_back(); }
    void pushClip(const std::vector<Vert>& verts);
    void popClip();
private:
    GLuint mTex0;
    GLuint mTexOut;
    GLuint mTexFront;
    GLuint mFbo[3];
    GLuint mRbo[2]; // For Stencil Buffers
    GLuint mVbo, mVao;
    std::vector<Mat4d> mTransforms { Mat4d::identity() };

};

bool quit;
SDL_Window *window;
SDL_GLContext glContext;

bool init() {
    //Use OpenGL 3.1 core
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    // Initialize video subsystem
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        // Display error message
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    // Create window
    window = SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,
                              SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        // Display error message
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Create OpenGL context
    if (glContext = SDL_GL_CreateContext(window); !glContext) {
        // Display error message
        printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }
    // Initialize glew
    glewInit();
}

void processEvent(const SDL_Event& ev) {
    switch (ev.type) {
        case SDL_QUIT:
            quit = true;
            break;
        default:
            break;
    }
}

void processEvents() {
    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent) != 0) {
        processEvent(sdlEvent);
    }
}

void render() {
    // Set background color as cornflower blue
    glClearColor(0.39f, 0.58f, 0.93f, 1.f);
    // Clear color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Update window with OpenGL rendering
    SDL_GL_SwapWindow(window);
};

void run() {
    quit = false;
    while (!quit) {
        processEvents();
        render();
    }
}

void finalize() {
    //Destroy window
    SDL_DestroyWindow(window);
    window = nullptr;
    //Quit SDL subsystems
    SDL_Quit();
}

#undef main

int main() {
    init();
    run();
    finalize();
    return 0;
}
