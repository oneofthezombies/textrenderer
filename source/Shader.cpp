#include "Shader.h"
#include <fstream>
#include <GL/glew.h>

void ShaderValueDeleter::operator()(ShaderValueType&& value) noexcept {
    glDeleteShader(value.get().get());
}

void ShaderProgramValueDeleter::operator()(ShaderProgramValueType&& value) noexcept {
    glDeleteProgram(value.get().get());
}

// Result<ShaderProgram> ShaderFactory::createShaderProgram(std::string&& vertexShaderPath, 
//                                                          std::string&& fragmentShaderPath) const noexcept {
//     std::ifstream vertexShaderFile(vertexShaderPath, std::ios::binary);
//     if (false == vertexShaderFile.is_open()) {
//         return Result<ShaderProgram>(Error(1, std::move(vertexShaderPath + " open failed.")));
//     }

//     // vertexShaderFile.seekg(0, std::ios::end);
//     // const int32_t vertexShaderCodeSize = vertexShaderFile.tellg();
//     // std::string vertexShaderCode(vertexShaderCodeSize, 0);
//     // vertexShaderFile.seekg(0);
//     // vertexShaderFile.read(vertexShaderCode.data(), vertexShaderCode.size());

//     // Shader vertexShader(glCreateShader(GL_VERTEX_SHADER));
//     // const char* vertexShaderCodePointer = vertexShaderCode.data();
//     // glShaderSource(vertexShader.get(), 1, &vertexShaderCodePointer, &vertexShaderCodeSize);
//     // glCompileShader(vertexShader.get());
//     // GLint vertexShaderCompileStatus = GL_FALSE;
//     // glGetShaderiv(vertexShader.get(), GL_COMPILE_STATUS, &vertexShaderCompileStatus);
//     // if (GL_FALSE == vertexShaderCompileStatus) {
//     //     GLint infoLogLength = 0;
//     //     glGetShaderiv(vertexShader.get(), GL_INFO_LOG_LENGTH, &infoLogLength);
//     //     std::string infoLog(infoLogLength, 0);
//     //     glGetShaderInfoLog(vertexShader.get(), infoLogLength, nullptr, infoLog.data());
//     //     return Result<ShaderProgram>(Error(1, std::move(infoLog)));
//     // }

//     // std::ifstream fragmentShaderFile(fragmentShaderPath, std::ios::binary);
//     // if (false == fragmentShaderFile.is_open()) {
//     //     return Result<ShaderProgram>(Error(1, std::move(fragmentShaderPath + " open failed.")));
//     // }

//     // fragmentShaderFile.seekg(0, std::ios::end);
//     // const int32_t fragmentShaderCodeSize = fragmentShaderFile.tellg();
//     // std::string fragmentShaderCode(fragmentShaderCodeSize, 0);
//     // fragmentShaderFile.seekg(0);
//     // fragmentShaderFile.read(fragmentShaderCode.data(), fragmentShaderCode.size());

//     // Shader fragmentShader(glCreateShader(GL_FRAGMENT_SHADER));
//     // const char* fragmentShaderCodePointer = fragmentShaderCode.data();
//     // glShaderSource(fragmentShader.get(), 1, &fragmentShaderCodePointer, &fragmentShaderCodeSize);
//     // glCompileShader(fragmentShader.get());
//     // GLint fragmentShaderCompileStatus = GL_FALSE;
//     // glGetShaderiv(fragmentShader.get(), GL_COMPILE_STATUS, &fragmentShaderCompileStatus);
//     // if (GL_FALSE == fragmentShaderCompileStatus) {
//     //     GLint infoLogLength = 0;
//     //     glGetShaderiv(fragmentShader.get(), GL_INFO_LOG_LENGTH, &infoLogLength);
//     //     std::string infoLog(infoLogLength, 0);
//     //     glGetShaderInfoLog(fragmentShader.get(), infoLogLength, nullptr, infoLog.data());
//     //     return Result<ShaderProgram>(Error(1, std::move(infoLog)));
//     // }

//     // ShaderProgram shaderProgram(glCreateProgram());
//     // glAttachShader(shaderProgram.get(), vertexShader.get());
//     // glAttachShader(shaderProgram.get(), fragmentShader.get());
//     // glLinkProgram(shaderProgram.get());
//     // GLint shaderProgramLinkStatus = GL_FALSE;
//     // glGetProgramiv(shaderProgram.get(), GL_LINK_STATUS, &shaderProgramLinkStatus);
//     // if (GL_FALSE == shaderProgramLinkStatus) {
//     //     GLint infoLogLength = 0;
//     //     glGetProgramiv(shaderProgram.get(), GL_INFO_LOG_LENGTH, &infoLogLength);
//     //     std::string infoLog(infoLogLength, 0);
//     //     glGetProgramInfoLog(shaderProgram.get(), infoLogLength, nullptr, infoLog.data());
//     //     return Result<ShaderProgram>(Error(1, std::move(infoLog)));
//     // }

//     // glDetachShader(shaderProgram.get(), vertexShader.get());
//     // glDetachShader(shaderProgram.get(), fragmentShader.get());

//     // return Result<ShaderProgram>(std::move(shaderProgram));
//     return Result<ShaderProgram>(Error(0, ""));
// }
