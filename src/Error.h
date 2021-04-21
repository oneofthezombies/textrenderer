#pragma once
#include <string>

struct Error {
    int32_t code = 0;
    std::string message;

    Error(int32_t code, std::string&& message) noexcept;
    Error(Error&& other) noexcept;
    Error& operator=(Error&& other) noexcept;

    Error(const Error&) = delete;
    Error& operator=(const Error& other) = delete;
};

std::ostream& operator<<(std::ostream& ostream, const Error& error) noexcept;
