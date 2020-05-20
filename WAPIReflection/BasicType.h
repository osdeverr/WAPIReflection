#pragma once
#include <cstdint>

namespace WAPIReflection {
    enum class BasicType : std::uint32_t
    {
        NoType = 0,
        Void = 1,
        Char = 2,
        WChar = 3,
        Int = 6,
        UInt = 7,
        Float = 8,
        BCD = 9,
        Bool = 10,
        Long = 13,
        ULong = 14,
        Currency = 25,
        Date = 26,
        Variant = 27,
        Complex = 28,
        Bit = 29,
        BSTR = 30,
        Hresult = 31
    };
}