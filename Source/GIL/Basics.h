#pragma once

#include <cstddef>
#include <array>

namespace Graphics {
    template <std::size_t ...idx>
    struct Layout {
        static constexpr std::size_t size = sizeof...(idx);
        static constexpr std::array<std::size_t, sizeof...(idx)> idxs = { idx ... };
        template <std::size_t index>
        static constexpr auto get() noexcept { return idxs[index]; }
    };

    template <class TColorFamily>
    struct ColorSpace {
        using ColorFamily = TColorFamily;
    };

    template <class TColorSpace, class TLayout>
    struct ColorInfoFactory {
        using ColorSpace = TColorSpace;
        using Layout = TLayout;
    };

    template <class ColorSpace, class Layout, class Target> struct InterfaceApplier;

    template <class TColorInfo, class Presentation>
    struct Color : InterfaceApplier<
            typename TColorInfo::ColorSpace,
            typename TColorInfo::Layout,
            typename Presentation::ColorPresentationProvider> {
        using InterfaceApplier<
                typename TColorInfo::ColorSpace,
                typename TColorInfo::Layout,
                typename Presentation::ColorPresentationProvider>
        ::template InterfaceApplier;
        using ColorInfo = TColorInfo;
    };

    template <class T>
    constexpr auto align() {
        auto res = (sizeof(T) / alignof(T)) * alignof(T);
        return res == sizeof(T) ? res : res + alignof(T);
    }

}
