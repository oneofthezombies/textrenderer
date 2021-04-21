#pragma once
#include <limits>
#include "Result.h"

class ShaderFactory;

class Shader {
public:
    Shader(Shader&& other) noexcept;
    ~Shader() noexcept;

    uint32_t get() const noexcept;

    void swap(Shader& other) noexcept;

    Shader& operator=(Shader&& other) noexcept;

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

private:
    Shader(uint32_t value) noexcept;

    uint32_t value_ = undefined;

    static constexpr uint32_t undefined = std::numeric_limits<uint32_t>::max();

    friend ShaderFactory;
};

namespace std {
    void swap(Shader& lhs, Shader& rhs) noexcept;
}

class ShaderProgram {
public:
    ShaderProgram(ShaderProgram&& other) noexcept;
    ~ShaderProgram() noexcept;

    uint32_t get() const noexcept;

    void swap(ShaderProgram& other) noexcept;

    ShaderProgram& operator=(ShaderProgram&& other) noexcept;

    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

private:
    ShaderProgram(uint32_t value) noexcept;

    uint32_t value_ = undefined;

    static constexpr uint32_t undefined = std::numeric_limits<uint32_t>::max();

    friend ShaderFactory;
};

namespace std {
    void swap(ShaderProgram& lhs, ShaderProgram& rhs) noexcept;
}

class ShaderFactory {
public:
    Result<ShaderProgram> createShaderProgram(std::string&& vertexShaderPath, 
                                              std::string&& fragmentShaderPath) const noexcept;
    Shader createVertexShader() const noexcept;
    Shader createFragmentShader() const noexcept;
};

class ShaderManager {
public:
    static Result<ShaderManager> open() noexcept;
};