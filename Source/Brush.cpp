//
// Created by dtzx2 on 2018/4/10.
//

#include "Brush.h"

TestBrush::TestBrush() {
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
}

void TestBrush::useBrush(const Mat4f& mvp) {
    mProg.use();
    glUniformMatrix4fv(mMvp, 1, GL_TRUE, mvp.data);
}
