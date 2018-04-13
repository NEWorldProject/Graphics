#pragma once

#include "Brush.h"
#include "Rect.h"
#include "EFX.h"

using Vert = Vec2d;

class ISprite : public NonCopyable {
public:
    virtual Rect getRect() const noexcept = 0;
    virtual BrushHandle getBrush() const = 0;
    virtual EFXHandle getEFX() const = 0;
    virtual const std::vector<Vert>& getVertices() const noexcept = 0;
};

class Sprite: public ISprite {
public:
    Sprite() = default;
    ~Sprite() = default;
    void setRect(const Rect& rect) noexcept { mRect = rect; }
    Rect getRect() const noexcept override { return mRect; }
    void setEFX(EFXHandle efx) { mEFX = std::move(efx); }
    EFXHandle getEFX() const override { return mEFX; }
    void setBrush(BrushHandle brush) { mBrush = std::move(brush); }
    BrushHandle getBrush() const override { return mBrush; }
    void setVertices(const std::vector<Vert>& in) { mVertices = in; }
    const std::vector<Vert>& getVertices() const noexcept override { return mVertices; }
private:
    Rect mRect;
    EFXHandle mEFX { nullptr };
    BrushHandle mBrush { nullptr };
    std::vector<Vert> mVertices;
};
