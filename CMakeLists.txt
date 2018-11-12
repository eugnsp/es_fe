cmake_minimum_required(VERSION 3.5)
project(es_fe_lib VERSION 1.0 DESCRIPTION "Finite elements library")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
#add_compile_options(-Wall -Wextra -Wpedantic -g -m64 -D_DEBUG)
add_compile_options(-Wall -Wextra -Wpedantic -m64 -O3 -DNDEBUG -flto -fconcepts)

add_library(es_fe_lib INTERFACE IMPORTED)
set_target_properties(es_fe_lib PROPERTIES
	INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_library(es_util_lib INTERFACE IMPORTED)
set_target_properties(es_util_lib PROPERTIES
	INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/../es_util/include")

add_library(es_la_lib INTERFACE IMPORTED)
set_target_properties(es_la_lib PROPERTIES
	INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/../es_la/include")

add_library(es_geom_lib STATIC IMPORTED)
set_target_properties(es_geom_lib PROPERTIES
	IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/../es_geom/build/libes_geom.a"
	INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/../es_geom/include")

include_directories("/opt/intel/compilers_and_libraries_2019.0.117/linux/mkl/include")
#link_directories("/opt/intel/compilers_and_libraries_2019.0.117/linux/mkl/lib/intel64")

file(GLOB_RECURSE SRC ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_library(es_fe STATIC "${SRC}")
#set_target_properties(es_fe PROPERTIES VERSION ${PROJECT_PROPERTIES})
target_link_libraries(es_fe es_util_lib es_la_lib es_geom_lib es_fe_lib)
