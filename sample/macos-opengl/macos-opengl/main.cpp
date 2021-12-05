//
//  main.cpp
//  macos-opengl
//
//  Created by hunhoe kim on 2021/12/05.
//

#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>

#include "ignore_warning/push.h"
#include "ignore_warning/Wdocumentation.h"
#include "ignore_warning/Wdocumentation-deprecated-sync.h"
#include <GLFW/glfw3.h>
#include "ignore_warning/pop.h"

#include "ignore_warning/push.h"
#include "ignore_warning/Wdeprecated-volatile.h"
#include <glm/glm.hpp>
#include "ignore_warning/pop.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Type.hpp"
#include "Result.hpp"

#include "ignore_warning/push.h"
#include "ignore_warning/Wdocumentation.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include "ignore_warning/pop.h"

struct StringStream {
    StringStream() noexcept {}
    
    template<typename Type>
    StringStream& operator<<(const Type& value) noexcept {
        stream_ << value;
        return *this;
    }
    
    String toString() const noexcept {
        return stream_.str();
    }
    
    explicit operator String() const noexcept {
        return toString();
    }
    
    std::stringstream stream_;
};

Result<GLuint> loadShader(const String& vertexShaderPath,
                          const String& fragmentShaderPath) {
    String vertexShaderCode;
    {
        std::ifstream vertexShaderFile(vertexShaderPath, std::ios::in);
        if (!vertexShaderFile.is_open()) {
            return Error(ErrorBuilder(-1) << "vertex shader file open failed. path: [" << vertexShaderPath << "]");
        }

        std::stringstream vertexShaderStream;
        vertexShaderStream << vertexShaderFile.rdbuf();
        vertexShaderCode = vertexShaderStream.str();
    }
    
    const GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    if (0 == vertexShaderId) {
        return Error(-1, "vertex shader creation failed.");
    }

    const char* vertexShaderCodePointer = vertexShaderCode.c_str();
    glShaderSource(vertexShaderId, 1, &vertexShaderCodePointer, nullptr);
    glCompileShader(vertexShaderId);
    {
        GLint result = GL_FALSE;
        glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &result);
        if (GL_FALSE == result) {
            GLint infoLogLength = 0;
            glGetShaderiv(vertexShaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
            if (0 == infoLogLength) {
                return Error(-1, "vertex shader compilation failed.");
            }

            String infoLog(infoLogLength + 1, '\0');
            glGetShaderInfoLog(vertexShaderId, infoLogLength, nullptr, infoLog.data());
            return Error(-1, std::move(infoLog));
        }
    }
    
    String fragmentShaderCode;
    {
        std::ifstream fragmentShaderFile(fragmentShaderPath, std::ios::in);
        if (!fragmentShaderFile.is_open()) {
            return Error(ErrorBuilder(-1) << "fragment shader file open failed. path: [" << fragmentShaderPath << "]");
        }

        std::stringstream fragmentShaderStream;
        fragmentShaderStream << fragmentShaderFile.rdbuf();
        fragmentShaderCode = fragmentShaderStream.str();
    }
    
    const GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    if (0 == fragmentShaderId) {
        return Error(-1, "fragment shader creation failed.");
    }

    const char* fragmentShaderCodePointer = fragmentShaderCode.c_str();
    glShaderSource(fragmentShaderId, 1, &fragmentShaderCodePointer, nullptr);
    glCompileShader(fragmentShaderId);

    {
        GLint result = GL_FALSE;
        glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &result);
        if (GL_FALSE == result) {
            GLint infoLogLength = 0;
            glGetShaderiv(fragmentShaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
            
            if (0 == infoLogLength) {
                return Error(-1, "fragment shader compilation failed.");
            }
            
            String infoLog(infoLogLength + 1, '\0');
            glGetShaderInfoLog(fragmentShaderId, infoLogLength, nullptr, infoLog.data());
            return Error(-1, std::move(infoLog));
        }
    }

    GLuint shaderProgramId = glCreateProgram();
    if (0 == shaderProgramId) {
        return Error(-1, "shader program creation failed.");
    }
    
    glAttachShader(shaderProgramId, vertexShaderId);
    glAttachShader(shaderProgramId, fragmentShaderId);
    glLinkProgram(shaderProgramId);

    {
        GLint result = GL_FALSE;
        glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &result);
        if (GL_FALSE == result) {
            GLint infoLogLength = 0;
            glGetProgramiv(shaderProgramId, GL_INFO_LOG_LENGTH, &infoLogLength);
            if (0 == infoLogLength) {
                return Error(-1, "shader program linking failed.");
            }
            
            String infoLog(infoLogLength + 1, '\0');
            glGetShaderInfoLog(shaderProgramId, infoLogLength, nullptr, infoLog.data());
            return Error(-1, std::move(infoLog));
        }
    }

    glDetachShader(shaderProgramId, vertexShaderId);
    glDetachShader(shaderProgramId, fragmentShaderId);

    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);

    return Result<GLuint>(std::move(shaderProgramId));
}

int main(int argc, const char* argv[]) {
    if (false == glfwInit()) {
        std::cerr << "failed to initialize glfw" << std::endl;
        return -1;
    }
    
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    const int32 screenWidth = 1024;
    const int32 screenHeight = 768;
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Tutorial 01", nullptr, nullptr);
    if (nullptr == window) {
        std::cerr << "failed to open glfw window." << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    
    glewExperimental = true;
    if (GLenum result = glewInit();
        GLEW_OK != result) {
        std::cerr << "failed to initialize glew" << std::endl;
        return -1;
    }
    
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    
    Result<GLuint> result = loadShader("/Users/hunhoekim/repo/textrenderer/graphics/opengl/shader/vertex.glsl", "/Users/hunhoekim/repo/textrenderer/graphics/opengl/shader/fragment.glsl");
    if (!result.isSuccess()) {
        std::cerr << result.getError().getMessage() << std::endl;
        return result.getError().getCode().get();
    }
    GLuint shaderProgramId = result.get();
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    GLuint vertexArrayId = 0;
    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

//    const GLfloat kVertexBufferData[] = {
//       -1.0f, -1.0f, 0.0f,
//       1.0f, -1.0f, 0.0f,
//       0.0f,  1.0f, 0.0f,
//    };
    
    const GLfloat kVertexBufferData[] = {
        -1.0f,-1.0f,-1.0f, // triangle 1 : begin
        -1.0f,-1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f, // triangle 1 : end
        1.0f, 1.0f,-1.0f, // triangle 2 : begin
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f, // triangle 2 : end
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f
    };
    
    GLuint vertexbuffer = 0;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(kVertexBufferData), kVertexBufferData, GL_STATIC_DRAW);
    
    const GLfloat kColorBufferData[] = {
        0.583f,  0.771f,  0.014f,
        0.609f,  0.115f,  0.436f,
        0.327f,  0.483f,  0.844f,
        0.822f,  0.569f,  0.201f,
        0.435f,  0.602f,  0.223f,
        0.310f,  0.747f,  0.185f,
        0.597f,  0.770f,  0.761f,
        0.559f,  0.436f,  0.730f,
        0.359f,  0.583f,  0.152f,
        0.483f,  0.596f,  0.789f,
        0.559f,  0.861f,  0.639f,
        0.195f,  0.548f,  0.859f,
        0.014f,  0.184f,  0.576f,
        0.771f,  0.328f,  0.970f,
        0.406f,  0.615f,  0.116f,
        0.676f,  0.977f,  0.133f,
        0.971f,  0.572f,  0.833f,
        0.140f,  0.616f,  0.489f,
        0.997f,  0.513f,  0.064f,
        0.945f,  0.719f,  0.592f,
        0.543f,  0.021f,  0.978f,
        0.279f,  0.317f,  0.505f,
        0.167f,  0.620f,  0.077f,
        0.347f,  0.857f,  0.137f,
        0.055f,  0.953f,  0.042f,
        0.714f,  0.505f,  0.345f,
        0.783f,  0.290f,  0.734f,
        0.722f,  0.645f,  0.174f,
        0.302f,  0.455f,  0.848f,
        0.225f,  0.587f,  0.040f,
        0.517f,  0.713f,  0.338f,
        0.053f,  0.959f,  0.120f,
        0.393f,  0.621f,  0.362f,
        0.673f,  0.211f,  0.457f,
        0.820f,  0.883f,  0.371f,
        0.982f,  0.099f,  0.879f
    };
    
    GLuint colorbuffer = 0;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(kColorBufferData), kColorBufferData, GL_STATIC_DRAW);
    
    const glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(screenWidth) / static_cast<float>(screenHeight), 0.1f, 100.0f);
    const glm::mat4 view = glm::lookAt(glm::vec3(4,3,3), glm::vec3(0,0,0), glm::vec3(0,1,0));
    const glm::mat4 model = glm::mat4(1.0f);
    const glm::mat4 mvp = projection * view * model;
    
    FT_Library lib;
    FT_Init_FreeType(&lib);
    
    while (true) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

        glUseProgram(shaderProgramId);

        const GLuint matrixId = glGetUniformLocation(shaderProgramId, "MVP");
        glUniformMatrix4fv(matrixId, 1, GL_FALSE, &mvp[0][0]);
        
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

//        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawArrays(GL_TRIANGLES, 0, 12 * 3);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glUseProgram(0);
        
        glfwSwapBuffers(window);
    
        glfwPollEvents();
        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)) {
            break;
        }
        
        if (GLFW_TRUE == glfwWindowShouldClose(window)) {
            break;
        }
    }
    
    return 0;
}
