#pragma once

#include "Brush.h"
#include "Rect.h"
#include "Effect.h"

using Vert = Vec2d;

class ISprite : public NonCopyableVirtualBase {
public:
    virtual Rect getRect() const noexcept = 0;
    virtual std::pair<const Vert*, int> getVertices() const noexcept = 0;
    void setEFX(EFXHandle efx) { mEFX = std::move(efx); }
    EFXHandle getEFX() const { return mEFX; }
    void setBrush(BrushHandle brush) { mBrush = std::move(brush); }
    BrushHandle getBrush() const { return mBrush; }
private:
    EFXHandle mEFX { nullptr };
    BrushHandle mBrush { nullptr };
};

class Sprite: public ISprite {
public:
    Sprite() = default;
    ~Sprite() = default;
    void setRect(const Rect& rect) noexcept { mRect = rect; }
    Rect getRect() const noexcept override { return mRect; }
    void setVertices(const std::vector<Vert>& in) { mVertices = in; }
    std::pair<const Vert*, int> getVertices() const noexcept override {
        return { mVertices.data(), mVertices.size()};
    }
private:
    Rect mRect;
    std::vector<Vert> mVertices;
};
