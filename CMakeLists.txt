cmake_minimum_required (VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

project (jsast)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

if (NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE "Debug")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
else()
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif(NOT CMAKE_BUILD_TYPE)

if (NOT WIN32)
	include(CheckCXXCompilerFlag)
	CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
	if(COMPILER_SUPPORTS_CXX14)
    		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")
	else()
    		message(FATAL_ERROR "Compiler ${CMAKE_CXX_COMPILER} has no C++14 support.")
	endif()
endif(NOT WIN32)

include_directories(./include)

set(JAST_SOURCE_FILES "")
set(JAST_HEADER_FILES "")
set(JAST_CODEGEN_FILES "")

add_subdirectory(./include/jast)
add_subdirectory(./src/jast)
add_subdirectory(./samples)
add_subdirectory(./tests)
add_subdirectory(./src/codegen)
add_subdirectory(./include/codegen)

add_library(jast STATIC ${JAST_HEADER_FILES} ${JAST_SOURCE_FILES})

# find LLVMConfig.cmake
find_package(LLVM REQUIRED CONFIG)
# find_package(LLVM REQUIRED COMPONENTS core native mcjit)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -DNO_EMIT_FUNCTION")

llvm_map_components_to_libnames(llvm_libs support core irreader mcjit native)

add_library(jast_codegen STATIC ${JAST_CODEGEN_FILES})
target_link_libraries(jast_codegen ${llvm_libs})


add_executable(cg ${CMAKE_CURRENT_SOURCE_DIR}/samples/codegen.cc)
target_link_libraries(cg jast jast_codegen ${llvm_libs})
