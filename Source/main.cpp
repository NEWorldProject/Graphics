#include <array>
#include "Graphics/GLUtils.h"
#include "Graphics/GraphicApplication.h"
#include "Graphics/Window.h"
#include "Graphics/Context.h"
#include "Graphics/Visual.h"

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// Shaders
const GLchar* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
out vec3 ourColor;
void main() {
    gl_Position = vec4(position, 1.0);
    ourColor = color;
})";
const GLchar* fragmentShaderSource = R"(
#version 330 core
in vec3 ourColor;
out vec4 color;
void main() {
    color = vec4(ourColor, 1.0f);
})";

class Renderer {
public:
    Renderer() : mWindow("Hello World!",
                         { Graphics::Window::PositionCentered, Graphics::Window::PositionCentered }, {WIDTH, HEIGHT}),
                 mCtx(mWindow), VBO(), MVP(), VAO(), mTriangle(), mFbo() {
        mWindow.show();
        glViewport(0, 0, WIDTH, HEIGHT);
        setUpShader();
        setUpBuffers();
        setUpTriangle();
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
        VAO.enableAttrib(0); // Position attribute
        VAO.enableAttrib(1); // Color attribute
        VAO.attribBinding(0, 0);
        VAO.attribBinding(1, 0);
        VAO.attribFormat(0, 3, GL_FLOAT, false, 0);
        VAO.attribFormat(1, 3, GL_FLOAT, false, 3 * sizeof(GLfloat));
        VAO.bindBuffer(0, VBO, 0, 6 * sizeof(GLfloat));
        MVP.bindBase(GL_UNIFORM_BUFFER, 0);
        MVP.data(mTransform.data, GL_DYNAMIC_DRAW);
    }

    void setUpTriangle() {
        constexpr std::array<GLfloat, 6> vertices = {
                0.5f, -0.5f,   // Bottom Right
                -0.5f, -0.5f,  // Bottom Left
                0.0f,  0.5f    // Top
        };
        mTriangle.setShape(vertices);
        mTriangle.setFillBrush(
                std::make_shared<Graphics::SolidColorBrush>(Graphics::RGBA::f32::Color(1.0f, 1.0f, 1.0f, 1.0f)));
        mTriangle.setBorderBrush(
                std::make_shared<Graphics::SolidColorBrush>(Graphics::RGBA::f32::Color(0.0f, 0.0f, 0.0f, 1.0f)));
    }

    void render() {
        mTransform = mTransform * Mat4f::rotation(1, {0.0f, 0.0f, 1.0f});
        MVP.dataSection(0, mTransform.data);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glLineWidth(4);
        mShader.use();
        VAO.use();
        glDrawArrays(GL_TRIANGLES, 0, 3);
        mTriangle.renderThis(mFbo);
    }

private:
    Graphics::Window mWindow;
    Graphics::Context mCtx;
    DataBuffer VBO, MVP;
    VertexArray VAO;
    Program mShader{};
    Mat4f mTransform = Mat4f::identity();
    Graphics::Triangle mTriangle;
    FrameBuffer mFbo;
};

class ApplicationTest : public Graphics::Application {
public:
    void run() override {
        Renderer renderer {};
        Application::run();
    }
};

DECL_APPLICATION(ApplicationTest)
