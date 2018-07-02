#pragma once

#include "Basics.h"
#include "AliasFactory.h"

namespace GIL {
    struct RGBAFamily;

    using ColorSpaceRGBA = ColorSpace<RGBAFamily>;

    using InfoRGBA = ColorInfoFactory<ColorSpaceRGBA, Layout<0, 1, 2, 3>>;
    using InfoBGRA = ColorInfoFactory<ColorSpaceRGBA, Layout<2, 1, 0, 3>>;
    using InfoARGB = ColorInfoFactory<ColorSpaceRGBA, Layout<1, 2, 3, 0>>;
    using InfoABGR = ColorInfoFactory<ColorSpaceRGBA, Layout<3, 2, 1, 0>>;

    template <class Layout, class Target>
    struct InterfaceApplier<ColorSpaceRGBA, Layout, Target> : Target {
        auto getR() const noexcept { return get<Layout::get<0>()>(); }
        auto getG() const noexcept { return get<Layout::get<1>()>(); }
        auto getB() const noexcept { return get<Layout::get<2>()>(); }
        auto getA() const noexcept { return get<Layout::get<3>()>(); }
        template <class T>
        auto& setR(T rhs) noexcept { set<Layout::get<0>()>(rhs); return *this; }
        template <class T>
        auto& setG(T rhs) noexcept { set<Layout::get<1>()>(rhs); return *this; }
        template <class T>
        auto& setB(T rhs) noexcept { set<Layout::get<2>()>(rhs); return *this; }
        template <class T>
        auto& setA(T rhs) noexcept { set<Layout::get<3>()>(rhs); return *this; }
    };

    using RGBA = BAInternalTypesAliasFactory<InfoRGBA>;
    using BGRA = BAInternalTypesAliasFactory<InfoBGRA>;
    using ARGB = BAInternalTypesAliasFactory<InfoARGB>;
    using ABGR = BAInternalTypesAliasFactory<InfoABGR>;

}
