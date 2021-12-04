#pragma once
#include "Result.hpp"
#include "Unique.hpp"

struct ShaderValueDeleter {
    void operator()(ShaderValueType&& value) noexcept;
};

template<>
struct UniqueTraits<ShaderValueType> {
    using value_type = ShaderValueType;
    using deleter_type = UniqueDeleterType<value_type>;

    static value_type getUndefinedValue() noexcept {
        return value_type(value_type::value_type::max());
    }

    static deleter_type getDeleter() noexcept {
        return ShaderValueDeleter();
    }
};

using Shader = Unique<ShaderValueType, UniqueTraits<ShaderValueType>>;

DEFINE_TYPE(ShaderProgramValueType, Uint32);

struct ShaderProgramValueDeleter {
    void operator()(ShaderProgramValueType&& value) noexcept;
};

// struct ShaderProgram : Unique<ShaderProgramValueType, ShaderProgramValueType(), ShaderProgramValueDeleter> {};

// struct ShaderFactory {
//     Result<ShaderProgram> createShaderProgram(std::string&& vertexShaderPath, 
//                                               std::string&& fragmentShaderPath) const noexcept;
// };
