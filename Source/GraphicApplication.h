#pragma once

#include <Core/Application.h>

namespace Graphics {
    class Application : public ::Application {
    public:
        Application();
        ~Application() override;
        void run() override;
        // Event System
        void pullEvents();
        void waitEvents();
        bool hasQueuingEvents();
    };
}