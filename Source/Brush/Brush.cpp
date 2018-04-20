//
// Created by dtzx2 on 2018/4/10.
//

#include "Brush.h"

namespace Brushes {
    class BrushDefault: public Brush {
    public:
        BrushDefault();
        void useBrush(const BrushCtx&) override;
    private:
        Program mProg;
        GLint mMvp;
        //GLint mRect;
    };
}

Brushes::BrushDefault::BrushDefault() {
    Shader vert{GL_VERTEX_SHADER, R"(
#version 330
in vec2 pos;
uniform mat4 mvp;
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
    mMvp = mProg.getUniformLoc("mvp");
    //mRect = mProg.getUniformLoc("rect");
}

void Brushes::BrushDefault::useBrush(const BrushCtx& ctx) {
    mProg.use();
    /*const auto& min = ctx.rect.min;
    const auto& max = ctx.rect.max;
    glUniform4f(mRect, static_cast<GLfloat>(min.x), static_cast<GLfloat>(min.y),
                static_cast<GLfloat>(max.x - min.x), static_cast<GLfloat>(max.y - min.y));*/
    glUniformMatrix4fv(mMvp, 1, GL_TRUE, ctx.mvp.data);
}

std::shared_ptr<Brush> Brush::getDefault() {
    static auto ins = std::make_shared<Brushes::BrushDefault>();
    return ins;
}
