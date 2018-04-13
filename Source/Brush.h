#pragma once

#include <memory>
#include "Utility.h"
#include "GLUtils.h"
#include "Math/Matrix.h"

class Brush: public NonCopyable {
public:
    virtual void useBrush(const Mat4f& mvp) = 0;
    virtual ~Brush() {}
};

using BrushHandle = std::shared_ptr<Brush>;

class TestBrush: public Brush {
public:
    TestBrush();
    void useBrush(const Mat4f& mvp) override;
private:
    Program mProg;
    GLint mMvp;
};