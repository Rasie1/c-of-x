include_directories(include)
file(GLOB_RECURSE SRC_LANG . src/*.cpp)

add_library(lang ${SRC_LANG})

enable_testing()

add_subdirectory(test)

add_test(lang LexerTests)
add_test(lang ParserTests)
add_test(lang LanguageTests)
