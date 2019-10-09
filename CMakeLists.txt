cmake_minimum_required(VERSION 3.13)
project(esf VERSION 1.0 DESCRIPTION "Finite elements library")

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE CPP_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")
add_library(esf STATIC "${CPP_FILES}")
target_include_directories(esf PUBLIC "include")
target_compile_features(esf PUBLIC cxx_std_17)
target_compile_options(esf PRIVATE -Wall -Wpedantic -Wextra -Werror=return-type -m64 -march=native 
					   $<$<CONFIG:DEBUG>:-O0 -g> $<$<CONFIG:RELEASE>:-Wno-unused-parameter -Wno-deprecated-declarations -O3 -DNDEBUG>)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	target_compile_options(esf PRIVATE -Wno-psabi)
endif()

if (NOT TARGET esu)
	add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/esu")
endif()
target_link_libraries(esf PUBLIC esu)

if (NOT TARGET esl)
	add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/esl")
endif()
target_link_libraries(esf PUBLIC esl)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads REQUIRED)
target_link_libraries(esf PUBLIC Threads::Threads)
