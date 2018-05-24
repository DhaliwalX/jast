include_directories(..)
add_executable(parse ${CMAKE_CURRENT_SOURCE_DIR}/parse.cc ${CMAKE_CURRENT_SOURCE_DIR}/dump-ast.cc)
target_link_libraries(parse jast)

