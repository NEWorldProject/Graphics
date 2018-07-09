#pragma once

#include "OpenGL.h"
#include "GIL/RGBA.h"

namespace Graphics {
    class BrushType {
    public:
        BrushType(const char* vert, const char* frag);
        void use() noexcept;
    protected:
        Program mShaderProgram;
    };

    class Brush {
    public:
        virtual void use() noexcept;
        virtual ~Brush();
    protected:
        constexpr explicit Brush(BrushType& type) noexcept;
    private:
        BrushType& mType;
    };

    class SolidColorBrush : public Brush {
    public:
        SolidColorBrush();
        explicit SolidColorBrush(const RGBA::f32::Color& color);
        void use() noexcept override;
        void setColor(const RGBA::f32::Color& color);
    private:
        DataBuffer mBrushData;
    };
}
