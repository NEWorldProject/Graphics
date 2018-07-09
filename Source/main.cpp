#include <array>
#include "Graphics/OpenGL.h"
#include "Graphics/GraphicApplication.h"
#include "Graphics/Window.h"
#include "Graphics/Visual.h"

// Window dimensions
const GLuint WIDTH = 600, HEIGHT = 600;

class Renderer {
public:
    Renderer() : mWindow("Hello World!",
                         { Graphics::Window::PositionCentered, Graphics::Window::PositionCentered }, {WIDTH, HEIGHT}),
                 mCtx(mWindow), mTriangle(), mFbo() {
        mWindow.show();
        glViewport(0, 0, WIDTH, HEIGHT);
        setUpTriangle();
        mCtx.unBindCurrent();
        mCtx.startRenderThread([this](){ render(); });
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
        mTriangle.setTransform(mTransform);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glLineWidth(4);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        Graphics::Visual::beginRender();
        mTriangle.renderThis(mFbo);
        Graphics::Visual::closeRender();
    }

private:
    Graphics::Window mWindow;
    Graphics::Context mCtx;
    Mat4f mTransform = Mat4f::identity();
    Graphics::Triangle mTriangle;
    Graphics::FrameBuffer mFbo;
};

class ApplicationTest : public Graphics::Application {
public:
    void run() override {
        Renderer renderer {};
        Application::run();
    }
};

DECL_APPLICATION(ApplicationTest)
