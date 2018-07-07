#pragma once

#include <Math/Vector.h>

namespace Graphics {
    class Window {
    public:
        enum PositionFlag : unsigned int {
            PositionCentered = 0x2fff0000, PositionDoNotCare = 0x1fff0000
        };
        Window() = default;
        Window(const std::string &title, Vec2ui initPos, Vec2ui initSize);
        Window(Window&& rhs) noexcept;
        Window& operator = (Window&& rhs) noexcept;
        Window(const Window&) = delete;
        Window& operator = (const Window&) = delete;
        ~Window() noexcept;
        void show() noexcept;
        void hide() noexcept;
        void raise() noexcept;
        void* raw() const noexcept { return mHdc; }
    private:
        void* mHdc = nullptr;
    };
}
