#pragma once
#include <es_fe/types.hpp>

#include <es_la/dense.hpp>

#include <memory>
#include <string>

namespace es_fe
{
std::unique_ptr<Mesh2> read_gmsh_file(const std::string& file_name, double scale = 1);

//la::Matrix<unsigned int, la::dynamic, 2> read_gmsh_file_tags(const std::string& file_name);
} // namespace es_fe