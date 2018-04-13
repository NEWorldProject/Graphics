#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include "Math/Matrix.h"
#include <SDL.h>
#include "GLUtils.h"
#include "Utility.h"
#include "Brush.h"
#include "Sprite.h"
#include "Scene.h"

bool quit;
SDL_Window *window;
SDL_GLContext glContext;
BrushHandle testBrush;

bool init() {
    // Initialize video subsystem
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        // Display error message
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    //Use OpenGL 3.1 core
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
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
    testBrush = std::make_shared<TestBrush>();
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
    // Set background color as cornflower blue

    glClearColor(0.7, 0.7, 1.0, 1.0);
    // Clear color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    static double frame = 0;
    frame += 5;
    scene.pushTransform(Mat4f::ortho(-300, 300, -300, 300, 0.0, 500.0)
                        * Mat4f::rotation(frame, {0, 0, 1})
                        * Mat4f::translation({100, 0, 0})
                        * Mat4f::rotation(frame * 7, {0, 0, 1}));
    Sprite sprite;
    std::vector<Vert> verts;
    verts.emplace_back(-25, -25);
    verts.emplace_back(25, -25);
    verts.emplace_back(0, 25);
    sprite.setVertices(verts);
    sprite.setRect({{-25, -25}, {25, 25}});
    sprite.setBrush(testBrush);
    scene.render(sprite, nullptr);
    scene.popTransform();
    // Update window with OpenGL rendering
    glBindFramebuffer(GL_READ_FRAMEBUFFER, scene.mFbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, 600, 600, 0, 0, 600, 600, GL_COLOR_BUFFER_BIT, GL_NEAREST);
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
