cmake_minimum_required(VERSION 3.16)

project(save_tool)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(save_tool
    src/main.cpp
    src/buffer.cpp
    src/save.cpp
)

target_include_directories(save_tool PRIVATE include)