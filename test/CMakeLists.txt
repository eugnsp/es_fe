cmake_minimum_required(VERSION 3.5)
project(es_fe_test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-Wall -Wextra -Wpedantic -D_DEBUG)

add_library(es_fe STATIC IMPORTED)
set_target_properties(es_fe PROPERTIES
	IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/../build/libes_fe.a"
	INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/../include")

add_library(es_util INTERFACE IMPORTED)
set_target_properties(es_util PROPERTIES
	INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/../../es_util/include")

add_library(es_la INTERFACE IMPORTED)
set_target_properties(es_la PROPERTIES
	INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/../../es_la/include")

add_library(es_geom STATIC IMPORTED)
set_target_properties(es_geom PROPERTIES
	IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/../../es_geom/build/libes_geom.a"
	INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/../../es_geom/include")

include_directories("/opt/intel/compilers_and_libraries_2019.0.117/linux/mkl/include")
link_directories("/opt/intel/compilers_and_libraries_2019.0.117/linux/mkl/lib/intel64")

set(MKL_LIBRARIES "-Wl,--start-group" libmkl_intel_ilp64.a libmkl_sequential.a libmkl_core.a "-Wl,--end-group" pthread m dl)

file(GLOB_RECURSE TEST_SRC RELATIVE "${PROJECT_SOURCE_DIR}/src" ${PROJECT_SOURCE_DIR}/src/*.cpp)
list(SORT TEST_SRC)

foreach(CPP_FILE ${TEST_SRC})
	get_filename_component(F_NAME ${CPP_FILE} NAME_WE)
	get_filename_component(F_DIR ${CPP_FILE} DIRECTORY)
	add_executable("${F_DIR}_${F_NAME}" "src/${CPP_FILE}")
	target_link_libraries("${F_DIR}_${F_NAME}" es_util es_la es_fe es_geom ${MKL_LIBRARIES})
endforeach()

