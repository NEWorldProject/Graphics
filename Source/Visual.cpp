#include "Graphics/Visual.h"

namespace {
    class VisualRenderContext {
    public:
        VisualRenderContext() {
            mVa.enableAttrib(0); // Position attribute
            mVa.attribBinding(0, 0);
            mVa.attribFormat(0, 2, GL_FLOAT, false, 0);
        }
        void useTempBuffer() noexcept {
            mTempBuffer.use(GL_FRAMEBUFFER);
        }
        void useVa() noexcept { mVa.use(); }
        auto& getVa() noexcept { return mVa; }
    private:
        VertexArray mVa;
        FrameBuffer mTempBuffer;
    };

    thread_local std::unique_ptr<VisualRenderContext> gVisualRenderContext { nullptr };

    VisualRenderContext& getCtx() {
        if (!gVisualRenderContext)
            gVisualRenderContext = std::make_unique<VisualRenderContext>();
        return *gVisualRenderContext;
    }
}

void Graphics::Visual::setOutEffect(std::shared_ptr<Graphics::OutEfx> newEffect) noexcept {
    mOutputEffect = std::move(newEffect);
}

void Graphics::Visual::renderThis(FrameBuffer &fbo) const {
    auto& ctx = getCtx();
    //if (mOutputEffect)
    this->renderContent();
    /*if (mOutputEffect){
        ctx.useTempBuffer();
    }*/
}

void Graphics::Visual::beginRender() noexcept {

}

void Graphics::Visual::closeRender() noexcept {

}

void Graphics::Triangle::setBorderBrush(std::shared_ptr<Graphics::Brush> newBrush) noexcept {
    mBorderBrush = std::move(newBrush);
}

void Graphics::Triangle::setFillBrush(std::shared_ptr<Graphics::Brush> newBrush) noexcept {
    mFillBrush = std::move(newBrush);
}

void Graphics::Triangle::renderContent() const {
    auto& ctx = getCtx();
    ctx.getVa().bindBuffer(0, mShape, 0, 2 * sizeof(GLfloat));
    ctx.useVa();
    if (mFillBrush) {
        mFillBrush->use();
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
    if (mBorderBrush) {
        mBorderBrush->use();
        glDrawArrays(GL_LINE_LOOP, 0, 3);
    }
}
