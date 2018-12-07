#pragma once
#include <es_fe/types.hpp>

#include <memory>
#include <string>
#include <vector>

namespace es_fe
{
std::unique_ptr<Mesh2> read_gmsh_mesh(const std::string& file_name, double scale = 1);
std::vector<unsigned int> read_gmsh_tags(const std::string& file_name, unsigned int tag_index);
} // namespace es_fe
