#pragma once
#include <es_la/dense.hpp>
#include <es/fe/forward.hpp>
#include <memory>
#include <string>

namespace fe
{
std::unique_ptr<Mesh2> read_gmsh_file(const std::string& file_name, double scale = 1);

la::Matrix<unsigned int, la::dynamic, 2> read_gmsh_file_tags(const std::string& file_name);
}