cmake_minimum_required(VERSION 3.14)
project(main LANGUAGES CXX)

include_directories(main 
    third-party/PEGTL/include
    third-party/robin-hood-hashing/src/include
    third-party/yomm2/include
)

add_executable(main 
    src/main.cpp
    src/parser.cpp
    src/operations.cpp
    src/eval.cpp
    src/apply.cpp
    src/unapply.cpp
    src/equality.cpp
    src/io.cpp
    src/ast_builder.cpp
    src/interpreter.cpp
    src/interpreter_test.cpp
)

target_link_libraries(main
    ${CMAKE_SOURCE_DIR}/third-party/yomm2/build/src/libyomm2.a
)

if (MSVC)
    target_compile_options(
        main
        PRIVATE
        /W4 /WX
    )
else ()
    target_compile_options(main 
        PRIVATE 
        -Wall -Wextra -pedantic -Werror -g -O0 -std=c++23
        -DBOOST_MOVE_USE_STANDARD_LIBRARY_MOVE)
endif ()