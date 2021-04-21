cmake_minimum_required(VERSION 3.0.0)
project(textrenderer VERSION 0.1.0)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

add_executable(${PROJECT_NAME} 
    src/main.cpp 
    src/Error.cpp
    src/App.cpp
    src/Shader.cpp
)

set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 11)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

target_link_libraries(${PROJECT_NAME} PRIVATE "-framework OpenGL")

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/thirdparty/glew-2.1.0/include")
add_library(glew SHARED IMPORTED)
set_property(TARGET glew PROPERTY IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/thirdparty/glew-2.1.0/build/cmake/build/lib/libGLEW.dylib")
target_link_libraries(${PROJECT_NAME} PRIVATE glew)

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/thirdparty/glfw-3.3.4/include")
add_library(glfw SHARED IMPORTED)
set_property(TARGET glfw PROPERTY IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/thirdparty/glfw-3.3.4/build/src/libglfw.dylib")
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/thirdparty/freetype-2.10.4/include")
add_library(freetype SHARED IMPORTED)
set_property(TARGET freetype PROPERTY IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/thirdparty/freetype-2.10.4/build/libfreetype.dylib")
target_link_libraries(${PROJECT_NAME} PRIVATE freetype)

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/thirdparty/harfbuzz-2.8.0/src")
add_library(harfbuzz SHARED IMPORTED)
set_property(TARGET harfbuzz PROPERTY IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/thirdparty/harfbuzz-2.8.0/build/libharfbuzz.dylib")
target_link_libraries(${PROJECT_NAME} PRIVATE harfbuzz)
