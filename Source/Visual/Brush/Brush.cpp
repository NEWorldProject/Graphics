//
// Created by dtzx2 on 2018/4/10.
//

#include "Visual/Brush.h"

namespace Brushes {
    class BrushTypeDefault : public BrushType {
    public:
        BrushTypeDefault() {
            auto& base = getBase();
            setNext(base.getNext());
            base.setNext(this);
        }
        void initialize() override {
            Shader vert{GL_VERTEX_SHADER, R"(
#version 330
in vec2 pos;
layout (std140) uniform vertexMvp {
    mat4 mvp;
};
void main() {
   gl_Position = mvp * vec4(pos.x, pos.y, 0.0, 1.0);
}
)"
            };
            Shader frag{GL_FRAGMENT_SHADER, R"(
#version 330
out vec4 outputColor;
void main() {
    outputColor = vec4(1.0, 1.0, 1.0, 1.0);
}
)"
            };
            mProg.link({&vert, &frag});

            GLuint blockIndex = glGetUniformBlockIndex(mProg.raw(), "vertexMvp");
            glUniformBlockBinding(mProg.raw(), blockIndex, 1);
        }

        void finalize() override {
        }

        void use() { mProg.use(); }

    private:
        Program mProg;
    } typeDefault;

    class BrushDefault: public Brush {
    public:
        BrushDefault();
        void useBrush() override;
    };
}

Brushes::BrushDefault::BrushDefault() = default;

void Brushes::BrushDefault::useBrush() {
    Brushes::typeDefault.use();
}

std::shared_ptr<Brush> Brush::getDefault() {
    static auto ins = std::make_shared<Brushes::BrushDefault>();
    return ins;
}

BrushType &BrushType::getBase() {
    static BrushType base;
    return base;
}

void BrushType::coInitialize() {
    auto current = BrushType::getBase().getNext();
    while (current){
        current->initialize();
        current = current->getNext();
    }
}

void BrushType::coFinalize() {
    auto current = BrushType::getBase().getNext();
    while (current){
        current->finalize();
        current = current->getNext();
    }
}
