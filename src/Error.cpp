#include "Error.h"
#include <ostream>

Error::Error(int32_t code, std::string&& message) noexcept
    : code(code)
    , message(std::move(message)) {
}

Error::Error(Error&& other) noexcept {
    std::swap(*this, other);
}

Error& Error::operator=(Error&& other) noexcept {
    std::swap(*this, other);
    return *this;
}

std::ostream& operator<<(std::ostream& ostream, const Error& error) noexcept {
    ostream << "[" << error.code << "] " << error.message;
    return ostream;
}

namespace std {
    void swap(Error& lhs, Error& rhs) noexcept {
        std::swap(lhs.code, rhs.code);
        std::swap(lhs.message, rhs.message);
    }
}
