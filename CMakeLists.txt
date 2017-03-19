cmake_minimum_required(VERSION 3.0)
set (PROJECT_NAME "Language" CXX)
project(${PROJECT_NAME})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)
option(COVERALLS "Generate coveralls data" OFF)

if (COVERALLS)
    include(Coveralls)
    coveralls_turn_on_coverage()
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -g -Wall -Wextra -Wno-unused-parameter")

enable_testing()

add_subdirectory(liblang)
add_subdirectory(repl)
add_subdirectory(stdlib)

if (COVERALLS)
    file(GLOB_RECURSE COVERAGE_SRCS . *.cpp)
    coveralls_setup("${COVERAGE_SRCS}" ON)
endif()
