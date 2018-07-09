#pragma once

#include <vector>
#include <memory>
#include "OutEfx.h"
#include "OpenGL.h"
#include "Brush.h"

namespace Graphics {
    class Visual {
    public:
        virtual void renderThis(FrameBuffer& fbo) const;
        void setOutEffect(std::shared_ptr<OutEfx> newEffect) noexcept;
        auto& getOutEffct() const noexcept { return mOutputEffect; }
        void setTransform(const Mat4f& transform) noexcept;
        auto& getTransform() const noexcept { return mTransform; }
        auto getParent() const noexcept { return mParent; }

        static void beginRender();

        static void closeRender();

    protected:
        void setParent(Visual* newParent) noexcept { mParent = newParent; }
        virtual void renderContent() const {};
        Mat4f mTransform;
    private:
        Visual* mParent { nullptr }, *mLeft { nullptr }, *mRight { nullptr };
        std::shared_ptr<OutEfx> mOutputEffect { nullptr };
    };

    class VisualRoot : public Visual {
    public:
        const std::unique_ptr<Visual> &getContent() const;
        void setContent(std::unique_ptr<Visual> newContent);
        void renderThis(FrameBuffer& fbo) const override;
    private:
        std::unique_ptr<Visual> mContent { nullptr };
    };

    class VisualCollectionNode : public Visual {
    public:
        auto& children() noexcept{ return mVisualChildren; }
        const auto& children() const noexcept { return mVisualChildren; }
        void renderThis(FrameBuffer& fbo) const override;
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