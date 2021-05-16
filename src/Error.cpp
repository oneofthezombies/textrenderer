#include "Error.h"
#include <ostream>

Error::Error(int32_t code, std::string&& message) noexcept
    : code(code)
    , message(std::move(message)) {
}

Error::Error(Error&& other) noexcept {
    swap(other);
}

Error& Error::operator=(Error&& other) noexcept {
    swap(other);
    return *this;
}

void Error::swap(Error& other) noexcept {
    std::swap(code, other.code);
    std::swap(message, other.message);
}

std::ostream& operator<<(std::ostream& ostream, const Error& error) noexcept {
    ostream << "[" << error.code << "] " << error.message;
    return ostream;
}
