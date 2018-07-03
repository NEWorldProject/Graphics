#include "GraphicContext.h"
#include "Window.h"
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <stdexcept>

namespace {
    auto upCast(void* hdc) { return reinterpret_cast<SDL_GLContext>(hdc); }
}

Graphics::Context::Context(Graphics::Window &window) {
    auto ctx = SDL_GL_CreateContext(reinterpret_cast<SDL_Window*>(window.raw()));
    if (!ctx)
        throw std::runtime_error(std::string("OpenGL context could not be created! SDL Error: ") + SDL_GetError());
    mHdc = ctx;
    mHost = window.raw();
    SDL_GL_SetSwapInterval(1);
    static bool isFirstContext = true;
    if (isFirstContext) {
        glewInit();
        isFirstContext = false;
    }
}

Graphics::Context::~Context() noexcept {
    if (mHdc) {
        if (mRenderFlag)
            closeRenderThread();
        SDL_GL_DeleteContext(upCast(mHdc));
        mHdc = nullptr;
    }
}

void Graphics::Context::makeCurrent() {
    SDL_GL_MakeCurrent(reinterpret_cast<SDL_Window*>(mHost), upCast(mHdc));
}

void Graphics::Context::startRenderThread(std::function<void()> renderFunction) {
    if (mRenderFlag.exchange(true))
        throw std::runtime_error("Render thread already running");
    mRenderThread = std::thread([this, renderFunction = std::move(renderFunction)]() {
        SDL_GL_MakeCurrent(reinterpret_cast<SDL_Window*>(mHost), upCast(mHdc));
        while (mRenderFlag) {
            renderFunction();
            SDL_GL_SwapWindow(reinterpret_cast<SDL_Window*>(mHost));
        }
    });
}

void Graphics::Context::closeRenderThread() noexcept {
    mRenderFlag = false;
    if (mRenderThread.joinable())
        mRenderThread.join();
}

void Graphics::Context::unBindCurrent() {
    SDL_GL_MakeCurrent(reinterpret_cast<SDL_Window*>(mHost), nullptr);
}

Graphics::Context::Context(Graphics::Context &&rhs) noexcept : mHdc(rhs.mHdc), mHost(rhs.mHost) {
    mHdc = mHost = nullptr;
}

Graphics::Context &Graphics::Context::operator=(Graphics::Context &&rhs) noexcept {
    std::swap(mHdc, rhs.mHdc);
    std::swap(mHost, rhs.mHost);
    return *this;
}
