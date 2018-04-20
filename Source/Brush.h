#pragma once

#include <memory>
#include "Utility.h"
#include "Common/Math/Matrix.h"
#include "Rect.h"
#include "GLUtils.h"

struct BrushCtx {
    const Mat4f& mvp;
    const Rect& rect;
    const Texture& prevRes;
};

class Brush: public NonCopyableVirtualBase {
public:
    virtual void useBrush(const BrushCtx&) = 0;
    virtual bool isBlender() const noexcept { return false; }
    static std::shared_ptr<Brush> getDefault();
};

using BrushHandle = std::shared_ptr<Brush>;
