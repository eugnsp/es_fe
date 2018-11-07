#pragma once
#include <stdexcept>
#include <string>

namespace fe
{
class Mesh_io_error : public std::runtime_error
{
public:
	Mesh_io_error(const std::string& error) : runtime_error(error)
	{ }
};
}
