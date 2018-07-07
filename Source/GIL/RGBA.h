#pragma once

#include "Basics.h"
#include "AliasFactory.h"

namespace Graphics {
    struct RGBAFamily;

    using ColorSpaceRGBA = ColorSpace<RGBAFamily>;

    using InfoRGBA = ColorInfoFactory<ColorSpaceRGBA, Layout<0, 1, 2, 3>>;
    using InfoBGRA = ColorInfoFactory<ColorSpaceRGBA, Layout<2, 1, 0, 3>>;
    using InfoARGB = ColorInfoFactory<ColorSpaceRGBA, Layout<1, 2, 3, 0>>;
    using InfoABGR = ColorInfoFactory<ColorSpaceRGBA, Layout<3, 2, 1, 0>>;

    template <class Layout, class Target>
    struct InterfaceApplier<ColorSpaceRGBA, Layout, Target> : Target {
        using Target::template Target;
        constexpr auto getR() const noexcept { return Target::template get<Layout::template get<0>()>(); }
        constexpr auto getG() const noexcept { return Target::template get<Layout::template get<1>()>(); }
        constexpr auto getB() const noexcept { return Target::template get<Layout::template get<2>()>(); }
        constexpr auto getA() const noexcept { return Target::template get<Layout::template get<3>()>(); }
        template <class T>
        auto& setR(T rhs) noexcept { Target::template set<Layout::template get<0>()>(rhs); return *this; }
        template <class T>
        auto& setG(T rhs) noexcept { Target::template set<Layout::template get<1>()>(rhs); return *this; }
        template <class T>
        auto& setB(T rhs) noexcept { Target::template set<Layout::template get<2>()>(rhs); return *this; }
        template <class T>
        auto& setA(T rhs) noexcept { Target::template set<Layout::get<3>()>(rhs); return *this; }
    };

    using RGBA = BAInternalTypesAliasFactory<InfoRGBA>;
    using BGRA = BAInternalTypesAliasFactory<InfoBGRA>;
    using ARGB = BAInternalTypesAliasFactory<InfoARGB>;
    using ABGR = BAInternalTypesAliasFactory<InfoABGR>;

}
