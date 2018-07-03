#include "GraphicApplication.h"
#include <SDL2/SDL.h>

namespace {
    bool appShouldExit = false;

    void handleEvent(const SDL_Event& ev) {
        switch (ev.type) {
            case SDL_QUIT:
                appShouldExit = true;
                break;
            default:
                break;
        }
    }
}

Graphics::Application::Application() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        throw std::runtime_error(std::string("SDL could not initialize! SDL_Error: ") + SDL_GetError());
    //Use OpenGL 3.1 core
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
}

Graphics::Application::~Application() {
    SDL_Quit();
}

void Graphics::Application::run() {
    while (!appShouldExit)
        waitEvents();
}

void Graphics::Application::pullEvents() {
    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent) != 0)
        handleEvent(sdlEvent);
}

void Graphics::Application::waitEvents() {
    SDL_Event sdlEvent;
    SDL_WaitEvent(&sdlEvent);
    handleEvent(sdlEvent);
    pullEvents();
}

bool Graphics::Application::hasQueuingEvents() {
    SDL_Event sdlEvent;
    return static_cast<bool>(SDL_PeepEvents(&sdlEvent, 1, SDL_PEEKEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT));
}
