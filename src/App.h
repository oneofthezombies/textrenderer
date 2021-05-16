#pragma once
#include <string>
#include <vector>
#include "Result.hpp"
#include "Shader.h"

struct GLFWwindow;

class AppFactory;

class App {
public:
    App(App&& other) noexcept;
    ~App() noexcept;

    App& operator=(App&& other) noexcept;

    App(const App&) = delete;
    App& operator=(const App&) = delete;

    void loop(const std::vector<ShaderProgram>& shaderPrograms) noexcept;

    void swap(App& other) noexcept;

    static void onResize(GLFWwindow* window, int32_t width, int32_t height) noexcept;

private:
    explicit App(GLFWwindow* window) noexcept;

    GLFWwindow* window_ = undefinedWindow;

    static constexpr GLFWwindow* undefinedWindow = std::numeric_limits<GLFWwindow*>::max();

    friend AppFactory;
};

class AppFactory {
public:
    Result<App> createApp(std::string&& title, int32_t screenWidth, int32_t screenHeight) noexcept;
};