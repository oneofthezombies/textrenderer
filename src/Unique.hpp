#pragma once

template<typename ValueType, ValueType UndefinedValue, typename DeleterType>
struct Unique {
    using value_type = ValueType;
    using deleter_type = DeleterType;

    static constexpr value_type undefined_value = UndefinedValue;

    Unique(value_type&& value) noexcept
        : value(std::move(value)) {
    }

    Unique(Unique&& other) noexcept
        : value(std::move(other.value)) {
        other.value = undefined_value;
    }

    ~Unique() noexcept {
        if (undefined_value != value) {
            deleter_type()(std::move(value));
            value = undefined_value;
        }
    }

    Unique& operator=(Unique&& other) noexcept {
        value = std::move(other.value);
        other.value = undefined_value;
        return *this;
    }

    Unique() = delete;
    Unique(const Unique&) = delete;
    Unique& operator=(const Unique&) = delete;

    const value_type& get() const noexcept {
        return value;
    }

private:
    value_type value;
};
