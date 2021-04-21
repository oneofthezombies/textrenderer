#pragma once
#include <string>
#include <vector>
#include "Result.h"

struct GLFWwindow;

class AppFactory;
class ShaderProgram;

class App {
public:
    App(App&& other) noexcept;
    ~App() noexcept;

    void loop(const std::vector<ShaderProgram>& shaderPrograms) noexcept;

    GLFWwindow* getWindow() const noexcept;

    void swap(App& other) noexcept;

    App& operator=(App&& other) noexcept;

    static void onResize(GLFWwindow* window, int32_t width, int32_t height) noexcept;

    App(const App&) = delete;
    App& operator=(const App&) = delete;

private:
    App(GLFWwindow* window) noexcept;

    GLFWwindow* window_ = undefinedWindow;

    static constexpr GLFWwindow* undefinedWindow = std::numeric_limits<GLFWwindow*>::max();

    friend AppFactory;
};

namespace std {
    void swap(App& lhs, App& rhs) noexcept;
}

class AppFactory {
public:
    Result<App> createApp(std::string&& title, int32_t screenWidth, int32_t screenHeight) noexcept;
};