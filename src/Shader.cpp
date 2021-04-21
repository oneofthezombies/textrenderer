#include "Shader.h"
#include <fstream>
#include <GL/glew.h>

Shader::Shader(uint32_t value) noexcept
    : value_(value) {
}

Shader::Shader(Shader&& other) noexcept {
    std::swap(*this, other);
}

Shader::~Shader() noexcept {
    if (undefined != value_) {
        glDeleteShader(value_);
    }
    value_ = undefined;
}

Shader& Shader::operator=(Shader&& other) noexcept {
    std::swap(*this, other);
    return *this;
}

uint32_t Shader::get() const noexcept {
    return value_;
}

void Shader::swap(Shader& other) noexcept {
    std::swap(value_, other.value_);
}

namespace std {
    void swap(Shader& lhs, Shader& rhs) noexcept {
        lhs.swap(rhs);
    }
}

ShaderProgram::ShaderProgram(uint32_t value) noexcept 
    : value_(value) {
}

ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept {
    std::swap(*this, other);
}

ShaderProgram::~ShaderProgram() noexcept {
    if (undefined != value_) {
        glDeleteProgram(value_);
    }
    value_ = undefined;
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept {
    std::swap(*this, other);
    return *this;
}

void ShaderProgram::swap(ShaderProgram& other) noexcept {
    std::swap(value_, other.value_);
}

namespace std {
    void swap(ShaderProgram& lhs, ShaderProgram& rhs) noexcept {
        lhs.swap(rhs);
    }
}

uint32_t ShaderProgram::get() const noexcept {
    return value_;
}

Shader ShaderFactory::createVertexShader() const noexcept {
    return Shader(glCreateShader(GL_VERTEX_SHADER));
}

Shader ShaderFactory::createFragmentShader() const noexcept {
    return Shader(glCreateShader(GL_FRAGMENT_SHADER));
}

Result<ShaderProgram> ShaderFactory::createShaderProgram(std::string&& vertexShaderPath, 
                                                         std::string&& fragmentShaderPath) const noexcept {
    std::ifstream vertexShaderFile(vertexShaderPath, std::ios::binary);
    if (false == vertexShaderFile.is_open()) {
        return Result<ShaderProgram>(Error(1, std::move(vertexShaderPath + " open failed.")));
    }

    vertexShaderFile.seekg(0, std::ios::end);
    const int32_t vertexShaderCodeSize = vertexShaderFile.tellg();
    std::string vertexShaderCode(vertexShaderCodeSize, 0);
    vertexShaderFile.seekg(0);
    vertexShaderFile.read(vertexShaderCode.data(), vertexShaderCode.size());

    Shader vertexShader = createVertexShader();
    const char* vertexShaderCodePointer = vertexShaderCode.data();
    glShaderSource(vertexShader.get(), 1, &vertexShaderCodePointer, &vertexShaderCodeSize);
    glCompileShader(vertexShader.get());
    GLint vertexShaderCompileStatus = GL_FALSE;
    glGetShaderiv(vertexShader.get(), GL_COMPILE_STATUS, &vertexShaderCompileStatus);
    if (GL_FALSE == vertexShaderCompileStatus) {
        GLint infoLogLength = 0;
        glGetShaderiv(vertexShader.get(), GL_INFO_LOG_LENGTH, &infoLogLength);
        std::string infoLog(infoLogLength, 0);
        glGetShaderInfoLog(vertexShader.get(), infoLogLength, nullptr, infoLog.data());
        return Result<ShaderProgram>(Error(1, std::move(infoLog)));
    }

    std::ifstream fragmentShaderFile(fragmentShaderPath, std::ios::binary);
    if (false == fragmentShaderFile.is_open()) {
        return Result<ShaderProgram>(Error(1, std::move(fragmentShaderPath + " open failed.")));
    }

    fragmentShaderFile.seekg(0, std::ios::end);
    const int32_t fragmentShaderCodeSize = fragmentShaderFile.tellg();
    std::string fragmentShaderCode(fragmentShaderCodeSize, 0);
    fragmentShaderFile.seekg(0);
    fragmentShaderFile.read(fragmentShaderCode.data(), fragmentShaderCode.size());

    Shader fragmentShader = createFragmentShader();
    const char* fragmentShaderCodePointer = fragmentShaderCode.data();
    glShaderSource(fragmentShader.get(), 1, &fragmentShaderCodePointer, &fragmentShaderCodeSize);
    glCompileShader(fragmentShader.get());
    GLint fragmentShaderCompileStatus = GL_FALSE;
    glGetShaderiv(fragmentShader.get(), GL_COMPILE_STATUS, &fragmentShaderCompileStatus);
    if (GL_FALSE == fragmentShaderCompileStatus) {
        GLint infoLogLength = 0;
        glGetShaderiv(fragmentShader.get(), GL_INFO_LOG_LENGTH, &infoLogLength);
        std::string infoLog(infoLogLength, 0);
        glGetShaderInfoLog(fragmentShader.get(), infoLogLength, nullptr, infoLog.data());
        return Result<ShaderProgram>(Error(1, std::move(infoLog)));
    }

    ShaderProgram shaderProgram(glCreateProgram());
    glAttachShader(shaderProgram.get(), vertexShader.get());
    glAttachShader(shaderProgram.get(), fragmentShader.get());
    glLinkProgram(shaderProgram.get());
    GLint shaderProgramLinkStatus = GL_FALSE;
    glGetProgramiv(shaderProgram.get(), GL_LINK_STATUS, &shaderProgramLinkStatus);
    if (GL_FALSE == shaderProgramLinkStatus) {
        GLint infoLogLength = 0;
        glGetProgramiv(shaderProgram.get(), GL_INFO_LOG_LENGTH, &infoLogLength);
        std::string infoLog(infoLogLength, 0);
        glGetProgramInfoLog(shaderProgram.get(), infoLogLength, nullptr, infoLog.data());
        return Result<ShaderProgram>(Error(1, std::move(infoLog)));
    }

    glDetachShader(shaderProgram.get(), vertexShader.get());
    glDetachShader(shaderProgram.get(), fragmentShader.get());

    return Result<ShaderProgram>(std::move(shaderProgram));
}

Result<ShaderManager> ShaderManager::open() noexcept {
    glewExperimental = GL_TRUE;
    if (const GLenum errorCode = glewInit()) {
        return Result<ShaderManager>(Error(static_cast<int32_t>(errorCode), 
                                    std::string(reinterpret_cast<const char*>(glewGetErrorString(errorCode)))));
    } 

    return Result<ShaderManager>(ShaderManager{});
}