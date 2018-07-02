#pragma once

#include <cstddef>
#include <array>

namespace GIL {
    template <class Type, std::size_t size>
    struct BAColorPresentationProvider {
        static constexpr std::size_t packedSize = sizeof Type * size;
        static constexpr std::size_t alignedSize = packedSize;
        std::array<Type, size> data;
        template <std::size_t index>
        const auto& get() const noexcept { return std::get<index>(data); }
        template <std::size_t index>
        auto& set(Type rhs) noexcept { std::get<index>(data) = rhs; return *this; }
    };

    template <class Type, std::size_t size>
    struct BAPresentation {
        using ColorPresentationProvider = BAColorPresentationProvider<Type, size>;
    };
}