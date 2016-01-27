project(Language)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")
cmake_minimum_required(VERSION 3.0)

file(GLOB SRC_REPL . Classes/REPL/*.cpp)
file(GLOB INC_REPL ./Classes/Language Classes/Language/*.h)
file(GLOB SRC_LANG . Classes/Language/*.cpp)
file(GLOB INC_LANG . Classes/Language/*.h)

include_directories(${INC_LANG})
include_directories(${INC_REPL})

add_library(LanguageLib ${SRC_LANG})
add_executable(Language ${SRC_REPL} ${INC_REPL})

target_link_libraries(Language LanguageLib)
