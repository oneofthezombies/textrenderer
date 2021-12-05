#pragma once
#include <variant>
#include "Error.hpp"

template<typename ValueType, typename ErrorType>
struct ResultBase {
    using self_type = ResultBase;
    using value_type = ValueType;
    using error_type = ErrorType;

    ResultBase(value_type&& value) noexcept
        : value_(std::move(value)) {}

    ResultBase(error_type&& error) noexcept
        : value_(std::move(error)) {}

    bool isSuccess() const noexcept {
        return nullptr != std::get_if<value_type>(&value_);
    }

    const value_type& get() const noexcept {
        return *std::get_if<value_type>(&value_);
    }

    value_type& get() noexcept {
        return *std::get_if<value_type>(&value_);
    }

    const error_type& getError() const noexcept {
        return *std::get_if<error_type>(&value_);
    }

    error_type& getError() noexcept {
        return *std::get_if<error_type>(&value_);
    }

    explicit operator bool() noexcept {
        return isSuccess();
    }

    ResultBase(const self_type&) = delete;
    self_type& operator=(const self_type&) = delete;

private:
    std::variant<value_type, error_type> value_; 
};

template<typename ValueType>
using Result = ResultBase<ValueType, Error>;
