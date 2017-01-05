set (PROJECT_NAME "Language" CXX)
project(${PROJECT_NAME})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g -Wall -Wextra")
cmake_minimum_required(VERSION 3.0)

# file(GLOB_RECURSE SRC_REPL . Classes/REPL/*.cpp)
# file(GLOB_RECURSE SRC_LANG . Classes/Language/*.cpp)

# set(INC_LANG ./Classes/Language/System
#              ./Classes/Language/Parser
#              ./Classes/Language/Expressions)
# set(INC_REPL ./Classes/REPL)

# set(INCLUDES ${INC_LANG} ${INC_REPL})

# include_directories(${INCLUDES})

enable_testing()

add_subdirectory(liblang)
add_subdirectory(repl)
add_subdirectory(stdlib)
