cmake_minimum_required(VERSION 3.14)
project(SnakeGame VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        latest
)
FetchContent_MakeAvailable(glfw)

find_package(OpenGL REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(SnakeGame 
    src/main.cpp 
    src/glad.c
    src/Shader.cpp
    src/Snake.cpp
)

target_link_libraries(SnakeGame
    PRIVATE
    glfw
    ${OpenGL_LIBRARIES}
)