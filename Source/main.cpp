#include <iostream>
#include <array>
#include <Core/Application.h>
#include "Visual/GLUtils.h"
#include "GraphicApplication.h"
#include "Window.h"
#include "GraphicContext.h"

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// Shaders
const GLchar* vertexShaderSource = "#version 330 core\n"
                                   "layout (location = 0) in vec3 position;\n"
                                   "layout (location = 1) in vec3 color;\n"
                                   "out vec3 ourColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "gl_Position = vec4(position, 1.0);\n"
                                   "ourColor = color;\n"
                                   "}\0";
const GLchar* fragmentShaderSource = "#version 330 core\n"
                                     "in vec3 ourColor;\n"
                                     "out vec4 color;\n"
                                     "void main()\n"
                                     "{\n"
                                     "color = vec4(ourColor, 1.0f);\n"
                                     "}\n\0";

class Renderer {
public:
    Renderer() : mWindow("Hello World!", { SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED }, {WIDTH, HEIGHT}),
                 mCtx(mWindow), VBO(), VAO() {
        mWindow.show();
        glViewport(0, 0, WIDTH, HEIGHT);
        setUpShader();
        setUpBuffers();
        mCtx.unBindCurrent();
        mCtx.startRenderThread([this](){ render(); });
    }

    void setUpShader() {
        Shader vertexShader{GL_VERTEX_SHADER, vertexShaderSource};
        Shader fragmentShader{GL_FRAGMENT_SHADER, fragmentShaderSource};
        mShader.link({&vertexShader, &fragmentShader});
    }

    void setUpBuffers() {
        // Set up vertex data (and buffer(s)) and attribute pointers
        constexpr std::array<GLfloat, 18> vertices = {
                // Positions         // Colors
                0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // Bottom Right
                -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // Bottom Left
                0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // Top
        };

        VBO.data(vertices, GL_STATIC_DRAW);
        VAO.use();
        VAO.enableAttrib(0); // Position attribute
        VAO.enableAttrib(1); // Color attribute
        VAO.attribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
        VAO.attribFormat(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat));
        VAO.attribBinding(0, 0);
        VAO.attribBinding(1, 0);
        VAO.bindBuffer(0, VBO, 0, 6 * sizeof(GLfloat));
    }

    void render() {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw the triangle
        mShader.use();
        VAO.use();
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
    }

private:
    Graphics::Window mWindow;
    Graphics::Context mCtx;
    DataBuffer VBO;
    VertexArray VAO;
    Program mShader{};
};

class ApplicationTest : public Graphics::Application {
public:
    void run() override {
        Renderer renderer {};
        Application::run();
    }
};

DECL_APPLICATION(ApplicationTest)
