#include "Graphics/Visual.h"

using namespace Graphics;

namespace {
    class VisualRenderContext {
    public:
        VisualRenderContext() {
            mVa.enableAttrib(0); // Position attribute
            mVa.attribBinding(0, 0);
            mVa.attribFormat(0, 2, GL_FLOAT, false, 0);
            mMatrix.data(mMvp.back().data, GL_STREAM_DRAW);
        }
        void useTempBuffer() noexcept {
            mTempBuffer.use(GL_FRAMEBUFFER);
        }
        void useVa() noexcept { mVa.use(); }
        auto& getVa() noexcept { return mVa; }
        void pushMatrix(const Mat4f& matrix) { mMvp.push_back(mMvp.back() * matrix); }
        void popMatrix() { mMvp.pop_back(); }
        void flushMatrix() { mMatrix.dataSection(0, mMvp.back().data); }
        void begin() { mMatrix.bindBase(GL_UNIFORM_BUFFER, 0); }
    private:
        std::vector<Mat4f> mMvp { Mat4f::identity() };
        VertexArray mVa;
        DataBuffer mMatrix;
        FrameBuffer mTempBuffer;
    };

    thread_local std::unique_ptr<VisualRenderContext> gVisualRenderContext { nullptr };

    VisualRenderContext& getCtx() {
        if (!gVisualRenderContext)
            gVisualRenderContext = std::make_unique<VisualRenderContext>();
        return *gVisualRenderContext;
    }
}

void Visual::setOutEffect(std::shared_ptr<OutEfx> newEffect) noexcept {
    mOutputEffect = std::move(newEffect);
}

void Visual::renderThis(FrameBuffer &fbo) const {
    auto& ctx = getCtx();
    if (mOutputEffect)
        ctx.useTempBuffer();
    this->renderContent();
    if (mOutputEffect){
    }
}

void Visual::setTransform(const Mat4f &transform) noexcept { mTransform = transform; }

void Visual::beginRender() {
    getCtx().begin();
}

void Visual::closeRender() {

}

void Triangle::setBorderBrush(std::shared_ptr<Brush> newBrush) noexcept {
    mBorderBrush = std::move(newBrush);
}

void Triangle::setFillBrush(std::shared_ptr<Brush> newBrush) noexcept {
    mFillBrush = std::move(newBrush);
}

void Triangle::renderContent() const {
    auto& ctx = getCtx();
    ctx.pushMatrix(mTransform);
    ctx.flushMatrix();
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
    ctx.popMatrix();
}

void VisualCollectionNode::renderThis(FrameBuffer& fbo) const {
    for (auto&& x : mVisualChildren)
        x->renderThis(fbo);
}

const std::unique_ptr<Visual> &VisualRoot::getContent() const {
    return mContent;
}

void VisualRoot::setContent(std::unique_ptr<Visual> newContent) {
    mContent = std::move(newContent);
}

void VisualRoot::renderThis(FrameBuffer& fbo) const {
    getCtx().begin();
    mContent->renderThis(fbo);
}
