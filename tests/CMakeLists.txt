include_directories(../)

set (TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/parser_unittest1.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/test_ast_matcher.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/common.h)

add_executable (unittest ${TEST_SOURCES})
target_link_libraries(unittest JAST)
