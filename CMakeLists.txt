cmake_minimum_required(VERSION 3.13)
project(es_fe VERSION 1.0 DESCRIPTION "Finite elements library")

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE ES_FE_SRC "${PROJECT_SOURCE_DIR}/src/*.cpp")
add_library(es_fe STATIC "${ES_FE_SRC}")
target_include_directories(es_fe PUBLIC "include")
target_compile_features(es_fe PUBLIC cxx_std_17)
target_compile_options(es_fe PRIVATE -Wall -Wpedantic -Wextra -Werror=return-type -m64 -march=native 
					   $<$<CONFIG:DEBUG>:-O0 -g> $<$<CONFIG:RELEASE>:-Wno-unused-parameter -Wno-deprecated-declarations -O3 -DNDEBUG>)

if (NOT TARGET es_util)
	add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/es_util")
endif()
target_link_libraries(es_fe PUBLIC es_util)

if (NOT TARGET es_la)
	add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/es_la")
endif()
target_link_libraries(es_fe PUBLIC es_la)
