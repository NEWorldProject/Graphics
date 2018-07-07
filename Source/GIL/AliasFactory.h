#pragma once

#include "BAPresentation.h"
#include "Basics.h"

namespace Graphics {
    template <class TColor>
    struct AliasFactory {
        using Color = TColor;
    };

    template <class ColorInfo, class Type>
    using BAAliasFactory = AliasFactory<Color<ColorInfo, BAPresentation<Type, ColorInfo::Layout::size>>>;

    template <class ColorInfo>
    struct BAInternalTypesAliasFactory {
    private:
        template <class Type>
        using FactoryForColor = BAAliasFactory<ColorInfo, Type>;
    public:
        using s8 = FactoryForColor<std::int8_t>;
        using s16 = FactoryForColor<std::int16_t>;
        using s32 = FactoryForColor<std::int32_t>;
        using s64 = FactoryForColor<std::int64_t>;
        using u8 = FactoryForColor<std::uint8_t>;
        using u16 = FactoryForColor<std::uint16_t>;
        using u32 = FactoryForColor<std::uint32_t>;
        using u64 = FactoryForColor<std::uint64_t>;
        using f32 = FactoryForColor<float>;
        using f64 = FactoryForColor<double>;
    };
}
