#pragma once
#include <functional>
#include <variant>
#include <sstream>
#include "Error.h"

template<typename T>
struct Result {
    Result(T&& data) noexcept;
    Result(Error&& error) noexcept;

    bool isSuccess() noexcept;
    const T& data() const noexcept;
    T& data() noexcept;
    const Error& error() const noexcept;
    Error& error() noexcept;

    Result& throwIfError();
    Result& onError(std::function<void(Result&)> onError) noexcept;
    Result& onSuccess(std::function<void(Result&)> onSuccess) noexcept;

    explicit operator bool() noexcept;

    Result(const Result&) = delete;
    Result& operator=(const Result&) = delete;

private:
    std::variant<T, Error> value_; 
};

template<typename T>
Result<T>::Result(T&& data) noexcept
    : value_(std::move(data)) {
}

template<typename T>
Result<T>::Result(Error&& error) noexcept
    : value_(std::move(error)) {
}

template<typename T>
Result<T>::operator bool() noexcept {
    return isSuccess();
}

template<typename T>
bool Result<T>::isSuccess() noexcept {
    return nullptr != std::get_if<T>(&value_);
}

template<typename T>
const T& Result<T>::data() const noexcept {
    return *std::get_if<T>(&value_);
}

template<typename T>
T& Result<T>::data() noexcept {
    return *std::get_if<T>(&value_);
}

template<typename T>
const Error& Result<T>::error() const noexcept {
    return *std::get_if<Error>(&value_);
}

template<typename T>
Error& Result<T>::error() noexcept {
    return *std::get_if<Error>(&value_);
}

template<typename T>
Result<T>& Result<T>::onError(std::function<void(Result<T>&)> onError) noexcept {
    if (false == isSuccess()) {
        onError(*this);
    }
    return *this;
}

template<typename T>
Result<T>& Result<T>::onSuccess(std::function<void(Result&)> onSuccess) noexcept {
    if (true == isSuccess()) {
        onSuccess(*this);
    }
    return *this;
}

template<typename T>
Result<T>& Result<T>::throwIfError() {
    if (false == isSuccess()) {
        std::stringstream stream;
        stream << error();
        throw std::runtime_error(stream.str());
    }
    return *this;
}
