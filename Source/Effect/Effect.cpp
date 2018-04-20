#include "Effect.h"

namespace Effects {
    class EffectDefault final : public Effect {
    public:
        EffectDefault();
        void use(Texture &last, const Mat4f &mat) override;
    private:
        Program mProg{};
        GLint mMvp{}, mLast{};
    };
}

Effects::EffectDefault::EffectDefault() {
    Shader vert{GL_VERTEX_SHADER, R"(
#version 330
in vec2 pos;
uniform mat4 mvp;
out vec2 texCrood;
void main() {
   gl_Position = mvp * vec4(pos.x, pos.y, 0.0, 1.0);
   texCrood = vec2((gl_Position.x + 1.0) / 2, (gl_Position.y + 1.0) / 2);
}
)"
    };
    Shader frag{GL_FRAGMENT_SHADER, R"(
#version 330
uniform sampler2D last;
in vec2 texCrood;
out vec4 outputColor;
void main() {
    outputColor = texture(last, texCrood);
}
)"
    };
    mProg.link({&vert, &frag});
    mMvp = mProg.getUniformLoc("mvp");
    mLast = mProg.getUniformLoc("last");
}

void Effects::EffectDefault::use(Texture &last, const Mat4f &mat) {
    mProg.use();
    last.use(GL_TEXTURE_2D);
    glUniform1i(mLast, GL_TEXTURE_2D);
    glUniformMatrix4fv(mMvp, 1, GL_TRUE, mat.data);
}

std::shared_ptr<Effect> Effect::getDefault() {
    static auto ins = std::make_shared<Effects::EffectDefault>();
    return ins;
}