set (PROJECT_NAME "Language" CXX)
project(${PROJECT_NAME})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g -Wall -Wextra")
cmake_minimum_required(VERSION 3.0)

enable_testing()

add_subdirectory(liblang)
add_subdirectory(repl)
add_subdirectory(stdlib)
