#pragma once

#include <Math/Vector.h>

namespace Graphics {
    class Window {
    public:
        Window() = default;
        Window(const std::string &title, Vec2ui initPos, Vec2ui initSize);
        Window(Window&& rhs) noexcept;
        Window& operator = (Window&& rhs) noexcept;
        Window(const Window&) = delete;
        Window& operator = (const Window&) = delete;
        ~Window() noexcept;
        void show();
        void hide();
        void* raw() const noexcept { return mHdc; }
    private:
        void* mHdc = nullptr;
    };
}
