set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
configure_file(prelude.lang
               "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/prelude.lang"
               COPYONLY)
