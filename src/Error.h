#pragma once
#include <string>

struct Error {
    Error(int32_t code, std::string&& message) noexcept;
    Error(Error&& other) noexcept;

    Error& operator=(Error&& other) noexcept;

    void swap(Error& other) noexcept;

    Error(const Error&) = delete;
    Error& operator=(const Error&) = delete;

    int32_t code = 0;
    std::string message;
};

std::ostream& operator<<(std::ostream& ostream, const Error& error) noexcept;
