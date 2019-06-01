#pragma once
#include <cassert>
#include <es_fe/mesh/mesh1.hpp>
#include <es_fe/types.hpp>
#include <es_la/io/matfile_writer.hpp>
#include <string>

namespace es_fe
{
class Matlab_writer1
{
public:
	Matlab_writer1(const std::string& file_name, const Mesh1& mesh, double mesh_scale = 1) :
		mat_file_(file_name), mesh_(mesh)
	{
		write_mesh(mesh_scale);
	}

	template<class Field>
	void write_vertex_field(const std::string& var_name, const Field& field)
	{
		assert(field.size() == *mesh_.n_vertices());
		mat_file_.write(var_name, field);
	}

	template<class Field>
	void write_edge_field(const std::string& var_name, const Field& field)
	{
		assert(field.size() == *mesh_.n_edges());
		mat_file_.write(var_name, field);
	}

	template<typename T>
	void write_scalar(const std::string& var_name, T value)
	{
		mat_file_.write(var_name, value);
	}

private:
	void write_mesh(double mesh_scale)
	{
		// Vertices
		es_la::Vector_xd vertices(*mesh_.n_vertices());
		for (es_fe::Vertex_index vertex{0}; vertex < mesh_.n_vertices(); ++vertex)
			vertices[*vertex] = mesh_.vertex(vertex).x() / mesh_scale;

		mat_file_.write("vertices", vertices);
	}

private:
	es_la::Matfile_writer mat_file_;
	const Mesh1& mesh_;
};
} // namespace es_fe
