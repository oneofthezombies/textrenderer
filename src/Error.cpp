#include "Error.h"
#include <ostream>

Error::Error(int32_t code, std::string&& message) noexcept
    : code(code)
    , message(std::move(message)) {
}

Error::Error(Error&& other) noexcept
    : code(other.code)
    , message(std::move(other.message)) {
}

Error& Error::operator=(Error&& other) noexcept {
    code = other.code;
    message = std::move(other.message);
    return *this;
}

std::ostream& operator<<(std::ostream& ostream, const Error& error) noexcept {
    ostream << "[" << error.code << "] " << error.message;
    return ostream;
}
