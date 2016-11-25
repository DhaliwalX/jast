include_directories(../)

set (TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/test_ast_matcher.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/common.h)

set (UNITTEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/parser_unittest1.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/common.h)

add_executable (asttest ${TEST_SOURCES})
add_executable (unittest ${UNITTEST_SOURCES})
target_link_libraries(unittest JAST)
target_link_libraries(asttest JAST)

set(TEST_SOURCE_FILES "")
include_directories(.)
add_subdirectory(./ast-builder)
add_subdirectory(./common)

add_executable(test-ast ${TEST_SOURCE_FILES})
target_link_libraries(test-ast JAST)
