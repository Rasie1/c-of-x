find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
add_definitions(-DBOOST_TEST_DYN_LINK)
file(GLOB SRC_TESTS . LanguageTests.cpp)
add_executable(LanguageTests ${SRC_TESTS})
target_link_libraries(LanguageTests
                      LanguageLib
                      ${Boost_FILESYSTEM_LIBRARY}
                      ${Boost_SYSTEM_LIBRARY}
                      ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                      )
