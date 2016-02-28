set (PROJECT_NAME "Language" CXX)
project (${PROJECT_NAME})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")
cmake_minimum_required(VERSION 3.0)

file(GLOB SRC_REPL . Classes/REPL/*.cpp)
file(GLOB INC_REPL ./Classes/Language Classes/Language/*.h)
file(GLOB SRC_LANG . Classes/Language/*.cpp Classes/Language/Parser/*.cpp)
file(GLOB INC_LANG . Classes/Language/*.h Classes/Language/Parser)


enable_testing()
add_subdirectory(Tests)
add_test (NAME MyTest COMMAND Tests)


find_package (Boost 1.55 REQUIRED COMPONENTS)
if (Boost_FOUND)
  include_directories (SYSTEM ${Boost_INCLUDE_DIR})
endif ()




include_directories(${INC_LANG})
include_directories(${INC_REPL})

add_library(LanguageLib ${SRC_LANG})
add_executable(${PROJECT_NAME} ${SRC_REPL} ${INC_REPL})

target_link_libraries(${PROJECT_NAME} LanguageLib)
target_link_libraries (${PROJECT_NAME} ${Boost_LIBRARIES})
