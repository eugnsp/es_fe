#include <es_fe/mesh/io/gmsh.hpp>
#include <es_fe/mesh.hpp>
#include <es_fe/mesh/exception.hpp>
#include <array>
#include <cstddef>
#include <fstream>
#include <limits>
#include <unordered_map>
#include <memory>
#include <string>

namespace es_fe
{
class Gmsh_file_reader
{
public:
	Gmsh_file_reader(const std::string& file_name) : file_(file_name)
	{
		if (!file_)
			throw Mesh_io_error("Mesh file '" + file_name + "' cannot be opened for reading");
	}

	std::unique_ptr<Mesh2> read_mesh(double scale)
	{
		auto mesh = std::make_unique<Mesh2>();

		std::string line;
		while (std::getline(file_, line))
		{
			if (line.find("$MeshFormat") == 0)
				read_mesh_format_section();
			else if (line.find("$Nodes") == 0)
				read_nodes_section(*mesh, scale);
			else if (line.find("$Elements") == 0)
				read_elements_section(*mesh);
		}

		return mesh;
	}

private:
	static constexpr unsigned int GMSH_ELEMENT_POINT = 15;
	static constexpr unsigned int GMSH_ELEMENT_LINE = 1;
	static constexpr unsigned int GMSH_ELEMENT_TRIANGLE = 2;

	template<typename... Ts>
	void read(Ts&... args)
	{
		(file_ >> ... >> args);
		if (file_.bad())
			throw Mesh_io_error("Error reading mesh file");
	}

	void ignore_line()
	{
		file_.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	void read_mesh_format_section()
	{
		std::string version;
		unsigned int file_type, data_size;
		read(version, file_type, data_size);

		if (version != "2.2")
			throw Mesh_io_error("Unsupported Gmsh mesh file version " + version);

		if (file_type != 0 || data_size != 8)
			throw Mesh_io_error("Unsupported Gmsh mesh file format");

		read_section_footer("$EndMeshFormat");
	}

	void read_nodes_section(Mesh2& mesh, double scale)
	{
		Index n_vertices;
		read(n_vertices);

		mesh.reserve(n_vertices);
		node_indices_map_.reserve(n_vertices);
		for (Index i = 0; i < n_vertices; ++i)
		{
			Index index;
			double x, y, z;
			read(index, x, y, z);

			if (z != 0)
				throw Mesh_io_error("Not 2D Gmsh mesh file");

			node_indices_map_.insert({index, Vertex_index{i}});
			mesh.add_vertex({x * scale, y * scale});
		}

		read_section_footer("$EndNodes");
	}

	void read_elements_section(Mesh2& mesh)
	{
		Index n_elements;
		read(n_elements);

		for (Index i = 0; i < n_elements; ++i)
		{
			Index index;
			unsigned int element_type;
			read(index, element_type);

			if (element_type != GMSH_ELEMENT_TRIANGLE)
			{
				ignore_line();
				continue;
			}

			unsigned int n_tags;
			read(n_tags);

			for (unsigned int t = 0; t < n_tags; ++t)
			{
				int ignore;
				file_ >> ignore;
			}
			
			Index v1, v2, v3;
			read(v1, v2, v3);
			const auto vi1 = node_indices_map_.at(v1);
			const auto vi2 = node_indices_map_.at(v2);
			const auto vi3 = node_indices_map_.at(v3);

			mesh.add_cell(vi1, vi2, vi3);
		}

		read_section_footer("$EndElements");
	}

	void read_section_footer(const std::string& footer)
	{
		std::string section_footer;
		read(section_footer);

		if (section_footer != footer)
			throw Mesh_io_error("Bad Gmsh mesh file");
	}

private:
	std::ifstream file_;
	std::unordered_map<Index, Vertex_index> node_indices_map_;
};

std::unique_ptr<Mesh2> read_gmsh_file(const std::string& file_name, double scale)
{
	Gmsh_file_reader reader(file_name);
	return reader.read_mesh(scale);
}

// TODO : remove
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
}