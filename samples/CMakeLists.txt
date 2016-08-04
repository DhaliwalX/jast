include_directories(..)
add_executable(parse ${CMAKE_CURRENT_SOURCE_DIR}/parse.cc)
target_link_libraries(parse JAST)

