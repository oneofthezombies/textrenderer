#pragma once
#include <sstream>
#include "Type.hpp"

template<typename CodeType, typename MessageType>
struct ErrorBase {
    using self_type = ErrorBase;
    using code_type = CodeType;
    using message_type = MessageType;

    ErrorBase(code_type&& code, message_type&& message) noexcept 
        : code_(std::move(code))
        , message_(std::move(message)) {}

    ErrorBase(self_type&& other) noexcept 
        : code_(std::move(other.code_))
        , message_(std::move(other.message_)) {}
    
    ErrorBase(code_type&& code) noexcept
        : code_(std::move(code)) {}

    self_type& operator=(self_type&& other) noexcept {
        code_ = std::move(other.code_);
        message_ = std::move(other.message_);
        return *this;
    }

    const code_type& getCode() const noexcept {
        return code_;
    }

    code_type& getCode() noexcept {
        return code_;
    }

    const message_type& getMessage() const noexcept {
        return message_;
    }

    message_type& getMessage() noexcept {
        return message_;
    }
    
    ErrorBase(const self_type&) = delete;
    self_type& operator=(const self_type&) = delete;

    code_type code_;
    message_type message_;
};

using Error = ErrorBase<Int32, String>;

struct ErrorBuilder {
    using self_type = ErrorBuilder;
    using build_type = Error;
    
    ErrorBuilder(build_type::code_type&& code) noexcept
        : code_(std::move(code)) {}
    
    build_type build() noexcept {
        return build_type(std::move(code_), messageStream_.str());
    }

    template<typename MessageValueType>
    self_type& operator<<(const MessageValueType& messageValue) noexcept {
        messageStream_ << messageValue;
        return *this;
    }
    
    template<typename MessageValueType>
    self_type& operator<<(MessageValueType&& messageValue) noexcept {
        messageStream_ << messageValue;
        return *this;
    }
    
    explicit operator build_type() noexcept {
        return build();
    }
    
    ErrorBuilder(const self_type&) = delete;
    self_type& operator=(const self_type&) = delete;
    
private:
    build_type::code_type code_;
    std::stringstream messageStream_;
};
