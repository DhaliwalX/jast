include_directories(..)
add_executable(parse ${CMAKE_CURRENT_SOURCE_DIR}/parse.cc ${CMAKE_CURRENT_SOURCE_DIR}/pretty-printer.cc)
target_link_libraries(parse jast)

add_executable(cg ${CMAKE_CURRENT_SOURCE_DIR}/codegen.cc)
target_link_libraries(cg jast jast_codegen)
