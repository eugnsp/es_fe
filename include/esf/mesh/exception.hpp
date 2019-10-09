#pragma once
#include <stdexcept>
#include <string>

namespace es_fe
{
class Mesh_io_error : public std::runtime_error
{
public:
	Mesh_io_error(const std::string& error) : runtime_error(error)
	{}
};
} // namespace es_fe
