include_directories(include)
file(GLOB_RECURSE SRC_LANG . src/*.cpp)

add_library(lang ${SRC_LANG})

add_subdirectory(test)
