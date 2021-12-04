#pragma once
#include <cstdint>
#include <limits>
#include <type_traits>

using int8 = int8_t;
using uint8 = uint8_t;
using int16 = int16_t;
using uint16 = uint16_t;
using int32 = int32_t;
using uint32 = uint32_t;
using int64 = int64_t;
using uint64 = uint64_t;

template<
    typename ValueType, 
    typename NumberTraits,
    std::enable_if_t<std::is_arithmetic_v<ValueType>, bool> = true
>
struct Number {
    using self_type = Number;
    using value_type = ValueType;
    using traits_type = NumberTraits;

    Number() noexcept
        : value_(zero()) {}

    Number(const value_type& value) noexcept
        : value_(value) {}

    Number(value_type&& value) noexcept
        : value_(std::move(value)) {}

    Number(const self_type& other) noexcept
        : value_(other.value_) {}

    Number(self_type&& other) noexcept
        : value_(std::move(other.value_)) {}

    self_type& operator=(const self_type& other) noexcept {
        value_ = other.value_;
        return *this;
    }

    self_type& operator=(self_type&& other) noexcept {
        value_ = std::move(other.value_);
        return *this;
    }

    value_type get() const noexcept {
        return value_;
    }

    value_type operator*() const noexcept {
        return get();
    }

    explicit operator value_type() const noexcept {
        return value_;
    }

    static value_type zero() noexcept {
        return traits_type::zero();
    }

    static value_type min() noexcept {
        return traits_type::min();
    }

    static value_type max() noexcept {
        return traits_type::max();
    }

private:
    value_type value_;
};

template<typename ValueType>
struct NumberTraits {};

template<>
struct NumberTraits<bool> {
    using value_type = bool;
    static value_type zero() noexcept { return false; }
    static value_type min() noexcept { return false; }
    static value_type max() noexcept { return true; }
};

template<>
struct NumberTraits<int8> {
    using value_type = int8;
    static value_type zero() noexcept { return 0; }
    static value_type min() noexcept { return std::numeric_limits<value_type>::lowest(); }
    static value_type max() noexcept { return std::numeric_limits<value_type>::max(); }
};

template<>
struct NumberTraits<uint8> {
    using value_type = uint8;
    static value_type zero() noexcept { return 0; }
    static value_type min() noexcept { return std::numeric_limits<value_type>::lowest(); }
    static value_type max() noexcept { return std::numeric_limits<value_type>::max(); }
};

template<>
struct NumberTraits<int16> {
    using value_type = int16;
    static value_type zero() noexcept { return 0; }
    static value_type min() noexcept { return std::numeric_limits<value_type>::lowest(); }
    static value_type max() noexcept { return std::numeric_limits<value_type>::max(); }
};

template<>
struct NumberTraits<uint16> {
    using value_type = uint16;
    static value_type zero() noexcept { return 0; }
    static value_type min() noexcept { return std::numeric_limits<value_type>::lowest(); }
    static value_type max() noexcept { return std::numeric_limits<value_type>::max(); }
};

template<>
struct NumberTraits<int32> {
    using value_type = int32;
    static value_type zero() noexcept { return 0; }
    static value_type min() noexcept { return std::numeric_limits<value_type>::lowest(); }
    static value_type max() noexcept { return std::numeric_limits<value_type>::max(); }
};

template<>
struct NumberTraits<uint32> {
    using value_type = uint32;
    static value_type zero() noexcept { return 0; }
    static value_type min() noexcept { return std::numeric_limits<value_type>::lowest(); }
    static value_type max() noexcept { return std::numeric_limits<value_type>::max(); }
};

template<>
struct NumberTraits<int64> {
    using value_type = int64;
    static value_type zero() noexcept { return 0; }
    static value_type min() noexcept { return std::numeric_limits<value_type>::lowest(); }
    static value_type max() noexcept { return std::numeric_limits<value_type>::max(); }
};

template<>
struct NumberTraits<uint64> {
    using value_type = uint64;
    static value_type zero() noexcept { return 0; }
    static value_type min() noexcept { return std::numeric_limits<value_type>::lowest(); }
    static value_type max() noexcept { return std::numeric_limits<value_type>::max(); }
};

template<>
struct NumberTraits<float> {
    using value_type = float;
    static value_type zero() noexcept { return 0.0f; }
    static value_type min() noexcept { return std::numeric_limits<value_type>::lowest(); }
    static value_type max() noexcept { return std::numeric_limits<value_type>::max(); }
};

template<>
struct NumberTraits<double> {
    using value_type = double;
    static value_type zero() noexcept { return 0.0; }
    static value_type min() noexcept { return std::numeric_limits<value_type>::lowest(); }
    static value_type max() noexcept { return std::numeric_limits<value_type>::max(); }
};

using Bool = Number<bool, NumberTraits<bool>>;
using Int8 = Number<int8, NumberTraits<int8>>;
using Uint8 = Number<uint8, NumberTraits<uint8>>;
using Int16 = Number<int16, NumberTraits<int16>>;
using Uint16 = Number<uint16, NumberTraits<uint16>>;
using Int32 = Number<int32, NumberTraits<int32>>;
using Uint32 = Number<uint32, NumberTraits<uint32>>;
using Int64 = Number<int64, NumberTraits<int64>>;
using Uint64 = Number<uint64, NumberTraits<uint64>>;
using Float = Number<float, NumberTraits<float>>;
using Double = Number<double, NumberTraits<double>>;
