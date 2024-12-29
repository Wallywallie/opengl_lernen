#版本要求

cmake_minimum_required(VERSION 3.10)
# 项目名称和版本
project(opengl_learning VERSION 1.0)

#设置c++标准
set (CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 找到OpenGL
find_package(OpenGL REQUIRED)


set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")


message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
message(STATUS "PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")

set(INCLUDES_DIR "${PROJECT_SOURCE_DIR}/includes")

set(GLFW_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/dep/glfw/include")
set(GLFW_LIBRARY "${PROJECT_SOURCE_DIR}/dep/glfw/lib-mingw-w64/libglfw3.a")


set(GLAD_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/dep/glad/include")
set(GLAD_LIBRARY "${PROJECT_SOURCE_DIR}/dep/glad/src/glad.c")

# 设置头文件路径
include_directories(${GLFW_INCLUDE_DIR} ${INCLUDES_DIR})

#设置自定义的库
set(SOURCES
    src/lightshader.cpp
    src/stb_image.cpp
)

# 添加GLAD
add_library(GLAD STATIC ${GLAD_LIBRARY} ${SOURCES})
target_include_directories(GLAD PUBLIC ${GLAD_INCLUDE_DIR})

# 添加源文件
add_executable(draw src/main.cpp )


# 链接 GLFW 库到可执行文件
target_link_libraries(draw PRIVATE
    OpenGL::GL
    ${GLFW_LIBRARY}
    GLAD
)
