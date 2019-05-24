cmake_minimum_required(VERSION 3.7)
project(es_fe VERSION 1.0 DESCRIPTION "Finite elements library")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "-Wall -Wpedantic -Wextra -Werror=return-type -m64 -march=native -fmax-errors=5")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "-Wno-unused-parameter -Wno-deprecated-declarations -O3 -DNDEBUG")

file(GLOB_RECURSE ES_FE_SRC "${PROJECT_SOURCE_DIR}/src/*.cpp")
add_library(es_fe STATIC "${ES_FE_SRC}")
target_include_directories(es_fe PUBLIC "include")

if (NOT TARGET es_util)
	add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/es_util")
endif()
target_link_libraries(es_fe PUBLIC es_util)

if (NOT TARGET es_la)
	add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/es_la")
endif()
target_link_libraries(es_fe PUBLIC es_la)
