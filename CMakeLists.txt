cmake_minimum_required(VERSION 3.5)
project(engine)

cmake_policy(SET CMP0072 NEW)
set(CMAKE_BUILD_TYPE Debug)

find_package(OpenGL REQUIRED)

include_directories(libs)
include_directories(include)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE LIBS    ${CMAKE_CURRENT_SOURCE_DIR}/libs/*.c ${CMAKE_CURRENT_SOURCE_DIR}/libs/*.cpp)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/map.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
add_executable(engine ${LIBS} ${SOURCES})
target_link_libraries(engine glfw OpenGL::GL)
