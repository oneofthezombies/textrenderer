#pragma once
#include <limits>
#include "Result.hpp"
#include "Unique.hpp"

using ShaderValueType = uint32_t;

struct ShaderDeleter {
    void operator()(ShaderValueType&& value) noexcept;
};

using Shader = Unique<ShaderValueType, std::numeric_limits<ShaderValueType>::max(), ShaderDeleter>;

using ShaderProgramValueType = uint32_t;

struct ShaderProgramDeleter {
    void operator()(uint32_t&& value) noexcept;
};

using ShaderProgram = Unique<ShaderProgramValueType, std::numeric_limits<ShaderProgramValueType>::max(), ShaderProgramDeleter>;

struct ShaderFactory {
    Result<ShaderProgram> createShaderProgram(std::string&& vertexShaderPath, 
                                              std::string&& fragmentShaderPath) const noexcept;
};
