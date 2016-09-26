set (PROJECT_NAME "Language" CXX)
project(${PROJECT_NAME})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g -Wall")
cmake_minimum_required(VERSION 3.0)

file(GLOB_RECURSE SRC_REPL . Classes/REPL/*.cpp)
file(GLOB_RECURSE SRC_LANG . Classes/Language/*.cpp)

set(INC_LANG ./Classes/Language/System
             ./Classes/Language/Parser
             ./Classes/Language/Expressions)
set(INC_REPL ./Classes/REPL)

set(INCLUDES ${INC_LANG} ${INC_REPL})

include_directories(${INCLUDES})

enable_testing()
add_subdirectory(Tests)
add_test(NAME LexerTests COMMAND LexerTests)
add_test(NAME ParserTests COMMAND ParserTests)
add_test(NAME LanguageTests COMMAND LanguageTests)







find_package(Boost COMPONENTS program_options REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
if (Readline_INCLUDE_DIR)
    set(READLINE_FOUND TRUE)
else()
    find_path(Readline_ROOT_DIR
        NAMES include/readline/readline.h
    )

    find_path(Readline_INCLUDE_DIR
        NAMES readline/readline.h
        HINTS ${Readline_ROOT_DIR}/include
    )

    find_library(Readline_LIBRARY
        NAMES readline
        HINTS ${Readline_ROOT_DIR}/lib
    )

    if(Readline_INCLUDE_DIR AND Readline_LIBRARY AND Ncurses_LIBRARY)
      set(READLINE_FOUND TRUE)
    else(Readline_INCLUDE_DIR AND Readline_LIBRARY AND Ncurses_LIBRARY)
      FIND_LIBRARY(Readline_LIBRARY NAMES readline)
      include(FindPackageHandleStandardArgs)
      FIND_PACKAGE_HANDLE_STANDARD_ARGS(Readline DEFAULT_MSG Readline_INCLUDE_DIR Readline_LIBRARY )
      MARK_AS_ADVANCED(Readline_INCLUDE_DIR Readline_LIBRARY)
    endif(Readline_INCLUDE_DIR AND Readline_LIBRARY AND Ncurses_LIBRARY)

    mark_as_advanced(
        Readline_ROOT_DIR
        Readline_INCLUDE_DIR
        Readline_LIBRARY
    )
endif()



add_library(LanguageLib ${SRC_LANG})
add_executable(${PROJECT_NAME} ${SRC_REPL}) #why inc_repl is here

target_link_libraries(${PROJECT_NAME} LanguageLib)
target_link_libraries(${PROJECT_NAME} readline)
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

# copy standard modules into build directory

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
configure_file(Modules/prelude.txt
               "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/prelude.txt"
               COPYONLY)
