cmake_minimum_required(VERSION 3.16)
project(AutoMiddleware LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -O2")

include_directories(include)

file(GLOB_RECURSE SOURCES
    "src/*.cpp"
)

add_executable(auto_middleware ${SOURCES})
