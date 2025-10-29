cmake_minimum_required(VERSION 3.16)
project(AutoMiddleware LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -O2")

include_directories(include)

file(GLOB_RECURSE SOURCES
    "src/*.cpp"
)

add_executable(auto_middleware ${SOURCES})

# Find and link pthread
# Note: pthreads is a separate library (pthread.so) that must be explicitly linked
# Note: <thread> only provide declaration, but not implementation
find_package(Threads REQUIRED)
target_link_libraries(auto_middleware PRIVATE Threads::Threads)
