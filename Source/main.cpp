#include <iostream>
#include <vector>
#include <memory>
#include <chrono>
#include <algorithm>
#include "Math/Matrix.h"
#include <SDL.h>
#include <Core/Application.h>
#include "Visual/GLUtils.h"
#include "Visual/Brush.h"
#include "Visual/Sprite.h"
#include "Visual/Scene.h"

bool quit;
SDL_Window *window;
SDL_GLContext glContext;

bool init() {
    // Initialize video subsystem
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        // Display error message
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    //Use OpenGL 3.1 core
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    // Create window
    window = SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 600,
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
    SDL_GL_SetSwapInterval(1);
    Scene::instance().setSize({600, 600});
    return true;
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
    auto& scene = Scene::instance();
    static double frame = 0;
    frame += 5;
    scene.setTransform(Mat4f::ortho(-300, 300, -300, 300, 0.0, 500.0)
                       * Mat4f::rotation(frame, {0, 0, 1})
                       * Mat4f::translation({100, 0, 0})
                       * Mat4f::rotation(frame * 7, {0, 0, 1}).getTranspose());
    Sprite sprite;
    std::vector<Vert> verts;
    verts.emplace_back(-25, -25);
    verts.emplace_back(25, -25);
    verts.emplace_back(0, 25);
    sprite.setVertices(verts);
    sprite.setRect({{-25, -25}, {25, 25}});
    scene.prepare();
    scene.render(sprite);
    scene.setTransform(Mat4f::ortho(-300, 300, -300, 300, 0.0, 500.0)
                        * Mat4f::rotation(frame + 180, {0, 0, 1})
                        * Mat4f::translation({100, 0, 0})
                        * Mat4f::rotation(frame * 7, {0, 0, 1}));
    auto now = std::chrono::steady_clock::now();
    for (int i = 0; i < 1000; ++i)
        scene.render(sprite);
    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - now).count()
              << "ns" << std::endl;
    // Update window with OpenGL rendering
    scene.getResult().use(GL_READ_FRAMEBUFFER);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, 600, 600, 0, 0, 600, 600, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    scene.getResult().use(GL_FRAMEBUFFER);
    glClearColor(0.7, 0.7, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    SDL_GL_SwapWindow(window);
};

void loop() {
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

class ApplicationTest : public Application {
public:
    void run() override {
        init();
        loop();
        finalize();
    }
};

DECL_APPLICATION(ApplicationTest)
