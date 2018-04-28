#pragma once

#include <memory>
#include "Common/Utility.h"
#include "GLUtils.h"
#include "Common/Math/Matrix.h"

class Effect: public NonCopyableVirtualBase {
public:
    virtual ~Effect() = default;
    virtual void use() = 0;
    static std::shared_ptr<Effect> getDefault();
};

using EFXHandle = std::shared_ptr<Effect>;
