#include <es_fe/mesh/mesh2.hpp>
#include <es_fe/mesh/io/gmsh.hpp>
#include <es_fe/mesh/io/gmsh_file_reader.hpp>

#include <memory>
#include <string>

namespace es_fe
{
std::unique_ptr<Mesh2> read_gmsh_file(const std::string& file_name, double scale)
{
	internal::Gmsh_file_reader reader(file_name);
	return reader.read_mesh(scale);
}

// TODO : remove
/*
la::Matrix<unsigned int, la::dynamic, 2> read_gmsh_file_tags(const std::string& file_name)
{
	constexpr unsigned int GMSH_ELEMENT_POINT = 15;
	constexpr unsigned int GMSH_ELEMENT_LINE = 1;
	constexpr unsigned int GMSH_ELEMENT_TRIANGLE = 2;

	std::fstream file(file_name);
	if (!file.is_open())
		throw Mesh_io_error("File '" + file_name + "' cannot be opened for reading");

	la::Matrix<unsigned int, la::dynamic, 2> tags;

	std::string line;
	while (std::getline(file, line))
	{
		if (line.find("$MeshFormat") == 0)
		{
			std::string version;
			unsigned int file_type;
			unsigned int data_size;

			file >> version >> file_type >> data_size;

			if (version != "2.2")
				throw std::runtime_error("Unsupported Gmsh file version " + version);

			if (file_type != 0 || data_size != 8)
				throw std::runtime_error("Unsupported Gmsh file format");
		}
		else if (line.find("$Elements") == 0)
		{
			Index n_elements;
			file >> n_elements;

			tags.resize(n_elements);

			Index i_tri = 0;
			for (Index i = 0; i < n_elements; ++i)
			{
				Index index;
				unsigned int element_type;

				file >> index >> element_type;
				if (element_type == GMSH_ELEMENT_TRIANGLE)
				{
					std::size_t n_tags;
					file >> n_tags;

					if (n_tags > 2)
						n_tags = 2;

					for (std::size_t t = 0; t < n_tags; ++t)
						file >> tags(i_tri, t);

					++i_tri;
				}

				file.ignore(1000, '\n');
			}

			break;
		}
	}

	if (file.bad())
		throw std::runtime_error("Error reading mesh file '" + file_name);

	return tags;
}
*/
} // namespace es_fe