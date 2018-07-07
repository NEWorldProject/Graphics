#pragma once

#include <vector>
#include <memory>
#include "OutEfx.h"
#include "GLUtils.h"
#include "Brush.h"

namespace Graphics {
    class Visual {
    public:
        void renderThis(FrameBuffer& fbo) const;
        void setOutEffect(std::shared_ptr<OutEfx> newEffect) noexcept;
        auto& getOutEffct() const noexcept { return mOutputEffect; }
        static void beginRender() noexcept;
        static void closeRender() noexcept;
    protected:
        virtual void renderContent() const = 0;
    private:
        std::shared_ptr<OutEfx> mOutputEffect { nullptr };
    };

    class VisualCollectionNode : public Visual {
    public:
        auto& children() noexcept{ return mVisualChildren; }
        const auto& children() const noexcept { return mVisualChildren; }
    private:
        std::vector<std::unique_ptr<Visual>> mVisualChildren;
    };

    class Triangle : public Visual {
    public:
        void setBorderBrush(std::shared_ptr<Brush> newBrush) noexcept;
        void setFillBrush(std::shared_ptr<Brush> newBrush) noexcept;
        auto& getBorderBrush() const noexcept { return mBorderBrush; }
        auto& getFillBrush() const noexcept { return mFillBrush; }
        template <class ContiguousContainer>
        void setShape(const ContiguousContainer& c) { mShape.data(c, GL_STATIC_DRAW); }
    private:
        void renderContent() const override;
        DataBuffer mShape;
        std::shared_ptr<Brush> mFillBrush { nullptr }, mBorderBrush { nullptr };
    };
}