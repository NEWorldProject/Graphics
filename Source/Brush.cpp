//
// Created by yshli on 2018/7/6.
//

#include "Graphics/Brush.h"

void Graphics::BrushType::use() noexcept {
    mShaderProgram.use();
}

Graphics::BrushType::BrushType(const char *vert, const char *frag) {
    Shader vertex (GL_VERTEX_SHADER, vert);
    Shader fragment (GL_FRAGMENT_SHADER, frag);
    mShaderProgram.link({&vertex, &fragment});
}

void Graphics::SolidColorBrush::setColor(const RGBA::f32::Color& color) {
    mBrushData.dataSection(0, color.data);
}

namespace {
    constexpr const char* solidColorBrushVert = R"(
#version 330 core
layout (location = 0) in vec2 position;
layout (std140) uniform vertexMvp {
    mat4 mvp;
};
void main() {
   gl_Position = mvp * vec4(position.x, position.y, 0.0, 1.0);
}
)";

    constexpr const char* solidColorBrushFrag = R"(
#version 330
layout (std140) uniform brushData {
    vec4 color;
};
out vec4 outputColor;
void main() {
    outputColor = color;
}
)";

    class SolidColorBrushType : public Graphics::BrushType {
    public:
        SolidColorBrushType() : BrushType(solidColorBrushVert, solidColorBrushFrag) {
            mShaderProgram.bindUniformBlock("vertexMvp", 0);
            mShaderProgram.bindUniformBlock("brushData", 1);
        }
        static auto& instance()  { static SolidColorBrushType ins; return ins; }
    };
}

Graphics::SolidColorBrush::SolidColorBrush() : Brush(SolidColorBrushType::instance()) {
    RGBA::f32::Color zeroColor (0.0f, 0.0f, 0.0f, 0.0f );
    mBrushData.data(zeroColor.data, GL_STATIC_DRAW);
}

Graphics::SolidColorBrush::SolidColorBrush(const RGBA::f32::Color &color) : Brush(SolidColorBrushType::instance()) {
    mBrushData.data(color.data, GL_STATIC_DRAW);
}

void Graphics::SolidColorBrush::use() noexcept {
    Brush::use();
    mBrushData.bindBase(GL_UNIFORM_BUFFER, 1);
}

Graphics::Brush::~Brush() = default;

constexpr Graphics::Brush::Brush(Graphics::BrushType &type) noexcept : mType(type) {}

void Graphics::Brush::use() noexcept { mType.use(); }
