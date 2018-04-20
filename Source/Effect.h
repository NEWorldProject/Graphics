#pragma once

#include <memory>
#include "Utility.h"
#include "GLUtils.h"
#include "Common/Math/Matrix.h"

class Effect: public NonCopyableVirtualBase {
public:
    virtual ~Effect() = default;
    virtual void use(Texture &last, const Mat4f &mat) = 0;
    static std::shared_ptr<Effect> getDefault();
};

using EFXHandle = std::shared_ptr<Effect>;
