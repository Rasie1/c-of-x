include_directories(include)
file(GLOB_RECURSE SRC_LANG . src/*.cpp)

add_library(lang ${SRC_LANG})

add_subdirectory(test)

enable_testing()

add_test(NAME LexerTests    COMMAND LexerTests)
add_test(NAME ParserTests   COMMAND ParserTests)
add_test(NAME LanguageTests COMMAND LanguageTests)
