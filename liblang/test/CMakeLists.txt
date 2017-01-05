find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})


add_definitions(-DBOOST_TEST_DYN_LINK)

file(GLOB SRC_TESTS . LanguageTests.cpp)
add_executable(LanguageTests ${SRC_TESTS})
target_link_libraries(LanguageTests
                      lang
                      ${Boost_FILESYSTEM_LIBRARY}
                      ${Boost_SYSTEM_LIBRARY}
                      ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                      )

add_definitions(-DBOOST_TEST_DYN_LINK)
file(GLOB SRC_TESTS . LexerTests.cpp)
add_executable(LexerTests ${SRC_TESTS})
target_link_libraries(LexerTests
                      lang
                      ${Boost_FILESYSTEM_LIBRARY}
                      ${Boost_SYSTEM_LIBRARY}
                      ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                      )

add_definitions(-DBOOST_TEST_DYN_LINK)
file(GLOB SRC_TESTS . ParserTests.cpp)
add_executable(ParserTests ${SRC_TESTS})
target_link_libraries(ParserTests
                      lang
                      ${Boost_FILESYSTEM_LIBRARY}
                      ${Boost_SYSTEM_LIBRARY}
                      ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                      )

add_test(lang LexerTests)
add_test(lang ParserTests)
add_test(lang LanguageTests)
