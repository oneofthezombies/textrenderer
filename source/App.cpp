#include "App.h"
#include <GL/glew.h>
// #define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#include "Shader.h"

App::App(GLFWwindow* window) noexcept 
    : window_(window) {
}

App::App(App&& other) noexcept {
    swap(other);
}

App::~App() noexcept {
    if (undefinedWindow != window_) {
        glfwTerminate();
    }
    window_ = undefinedWindow;
}

App& App::operator=(App&& other) noexcept {
    swap(other);
    return *this;
}

void App::swap(App& other) noexcept {
    std::swap(window_, other.window_);
}

void App::loop(const std::vector<ShaderProgram>& shaderPrograms) noexcept {
    while (GLFW_FALSE == glfwWindowShouldClose(window_)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for(const ShaderProgram& shaderProgram : shaderPrograms) {
            glUseProgram(shaderProgram.get());
            glUseProgram(0);
        }

        glfwSwapBuffers(window_);
        glfwPollEvents();
    }
}

void App::onResize(GLFWwindow* window, int32_t width, int32_t height) noexcept {
    glViewport(0, 0, width, height);
}

Result<App> AppFactory::createApp(std::string&& title, int32_t screenWidth, int32_t screenHeight) noexcept {
    if (GLFW_FALSE == glfwInit()) {
        return Result<App>(Error(1, "glfw initialization failed."));
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(screenWidth, 
                                          screenHeight, 
                                          title.c_str(), nullptr, nullptr);
    if (nullptr == window) {
        return Result<App>(Error(1, "glfw window creation failed."));
    }

    App app(window);
    glfwMakeContextCurrent(app.window_);
    glfwSetWindowSizeCallback(app.window_, App::onResize);

    glewExperimental = GL_TRUE;
    if (const GLenum errorCode = glewInit()) {
        return Result<App>(Error(static_cast<int32_t>(errorCode), 
                                 std::string(reinterpret_cast<const char*>(glewGetErrorString(errorCode)))));
    } 

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glViewport(0, 0, screenWidth, screenHeight);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return Result<App>(std::move(app));
}