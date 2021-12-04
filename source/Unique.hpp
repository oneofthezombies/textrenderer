#pragma once

template<typename ValueType>
using UniqueDeleterType = std::function<void(ValueType&&)>;

template<typename ValueType, typename UniqueTraits>
struct Unique {
    using self_type = Unique;
    using value_type = ValueType;
    using traits_type = UniqueTraits;
    using deleter_type = UniqueDeleterType<ValueType>;

    Unique(value_type&& value) noexcept
        : value_(std::move(value)) {}

    Unique(self_type&& other) noexcept
        : value_(std::move(other.value_)) {
        other.value_ = std::move(getUndefinedValue());
    }

    ~Unique() noexcept {
        if (getUndefinedValue() != value_) {
            getDeleter()(std::move(value_));
            value_ = std::move(getUndefinedValue());
        }
    }

    Unique& operator=(self_type&& other) noexcept {
        value_ = std::move(other.value_);
        other.value_ = std::move(getUndefinedValue());
        return *this;
    }

    const value_type& get() const noexcept {
        return value_;
    }

    static value_type getUndefinedValue() noexcept {
        return unique_traits_type::getUndefinedValue();
    }

    static deleter_type getDeleter() noexcept {
        return unique_traits_type::getDeleter();
    }

    Unique() = delete;
    Unique(const self_type&) = delete;
    self_type& operator=(const self_type&) = delete;

private:
    value_type value_;
};

template<typename ValueType>
struct UniqueTraits {};
