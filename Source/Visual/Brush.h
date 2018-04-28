#pragma once

#include <memory>
#include "Common/Utility.h"
#include "Common/Math/Matrix.h"
#include "Rect.h"
#include "GLUtils.h"

class BrushType {
public:
    virtual void initialize() {}
    virtual void finalize() {}
    BrushType* getNext() { return mNext; }
    void setNext(BrushType* type) { mNext = type; }
    static BrushType& getBase();
    static void coInitialize();
private:
    BrushType* mNext { nullptr };

    void coFinalize();
};

class Brush: public NonCopyableVirtualBase {
public:
    virtual void useBrush() = 0;
    virtual bool isBlender() const noexcept { return false; }
    static std::shared_ptr<Brush> getDefault();
};

using BrushHandle = std::shared_ptr<Brush>;
