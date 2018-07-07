#pragma once

#include <atomic>
#include <thread>
#include <functional>

namespace Graphics {
    class Window;

    class Context {
    public:
        Context() = default;
        explicit Context(Window& window);
        Context(Context&& rhs) noexcept;
        Context& operator = (Context&& rhs) noexcept;
        Context(const Context&) = delete;
        Context& operator = (const Context&) = delete;
        ~Context() noexcept;
        void unBindCurrent() noexcept;
        void makeCurrent() noexcept;
        void startRenderThread(std::function<void()> renderFunction);
        void closeRenderThread() noexcept;
        void* raw() const noexcept { return mHdc; }
    private:
        void* mHdc = nullptr,* mHost = nullptr;
        std::atomic_bool mRenderFlag { false };
        std::thread mRenderThread;
    };
}