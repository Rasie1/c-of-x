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
)

target_link_libraries(main
    ${CMAKE_SOURCE_DIR}/third-party/yomm2/build/src/libyomm2.a
)

if (MSVC)
    target_compile_options(
        main
        PRIVATE
        /W4 /WX
        /wd4459  # declaration of 'identifier' hides global declaration
    )
else ()
    target_compile_options(main PRIVATE -Wall -Wextra -pedantic -Werror -std=c++20)
endif ()