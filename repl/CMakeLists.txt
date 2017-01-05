include_directories(include)
file(GLOB_RECURSE SRC_REPL . src/*.cpp)

find_package(Boost COMPONENTS program_options REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIR})

add_executable(repl ${SRC_REPL})

target_link_libraries(
    repl
    lang
    readline
    ${Boost_LIBRARIES}
)
