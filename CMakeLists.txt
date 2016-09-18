set (PROJECT_NAME "Language" CXX)
project(${PROJECT_NAME})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g -Wall")
cmake_minimum_required(VERSION 3.0)

file(GLOB SRC_REPL . Classes/REPL/*.cpp)
file(GLOB INC_REPL ./Classes/Language)
file(GLOB SRC_LANG . Classes/Language/*.cpp Classes/Language/Parser/*.cpp)
file(GLOB INC_LANG . Classes/Language Classes/Language/Parser)


enable_testing()
add_subdirectory(Tests)
add_test(NAME LanguageTests COMMAND LanguageTests)

find_package(Boost COMPONENTS program_options REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIR})

include_directories(${INC_LANG})
include_directories(${INC_REPL})

add_library(LanguageLib ${SRC_LANG})
add_executable(${PROJECT_NAME} ${SRC_REPL} ${INC_REPL})

target_link_libraries(${PROJECT_NAME} LanguageLib)
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

# copy standard modules into build directory

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
configure_file(Modules/prelude.txt
               "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/prelude.txt"
               COPYONLY)
