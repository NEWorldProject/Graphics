#pragma once

#include "Brush.h"
#include "Rect.h"

using Vert = Vec2d;

class ISprite : public NonCopyable {
public:
    virtual Rect getRect() const noexcept = 0;
    virtual BrushHandle getBrush() const = 0;
    virtual const std::vector<Vert>& getVertices() const noexcept = 0;
};

class Sprite: public ISprite {
public:
    Sprite() = default;
    ~Sprite() = default;
    void setRect(const Rect& rect) noexcept { mRect = rect; }
    Rect getRect() const noexcept override { return mRect; }
    void setBrush(BrushHandle brush) { mBrush = move(brush); }
    BrushHandle getBrush() const override { return mBrush; }
    void setVertices(const std::vector<Vert>& in) { mVertices = in; }
    const std::vector<Vert>& getVertices() const noexcept override { return mVertices; }
private:
    Rect mRect;
    std::vector<Vert> mVertices;
    BrushHandle mBrush;
};
