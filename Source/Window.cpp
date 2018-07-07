#include "Graphics/Window.h"
#include <SDL2/SDL.h>

namespace {
    auto upCast(void* hdc) noexcept { return reinterpret_cast<SDL_Window*>(hdc); }
}

Graphics::Window::Window(const std::string &title, Vec2ui initPos, Vec2ui initSize) {
    auto window = SDL_CreateWindow(title.c_str(), initPos.x, initPos.y, initSize.x, initSize.y, SDL_WINDOW_OPENGL);
    if (!window)
        throw std::runtime_error(std::string("Window could not be created! SDL_Error:") + SDL_GetError());
    mHdc = window;
}

Graphics::Window::~Window() noexcept {
    if (mHdc) {
        SDL_DestroyWindow(upCast(mHdc));
        mHdc = nullptr;
    }
}

void Graphics::Window::show() noexcept { SDL_ShowWindow(upCast(mHdc)); }

void Graphics::Window::hide() noexcept { SDL_HideWindow(upCast(mHdc)); }

Graphics::Window::Window(Graphics::Window && rhs) noexcept : mHdc(rhs.mHdc) { rhs.mHdc = nullptr; }

Graphics::Window &Graphics::Window::operator=(Graphics::Window &&rhs) noexcept {
    std::swap(mHdc, rhs.mHdc);
    return *this;
}

void Graphics::Window::raise() noexcept { SDL_RaiseWindow(upCast(mHdc)); }
