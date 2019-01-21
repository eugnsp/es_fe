#pragma once
#include <es_fe/mesh/mesh2.hpp>
#include <es_fe/geom/linestring.hpp>
#include <es_fe/geom/algorithm.hpp>

#include <stdexcept>
#include <vector>
#include <utility>

namespace es_fe
{
namespace internal
{
template<class Vertex_fn, class Halfedge_fn>
void enumerate_elements_in_linestrip(
	const Linestring& linestring, const Mesh2& mesh, Vertex_fn vertex_fn, Halfedge_fn halfedge_fn)
{
	auto vertex_view = mesh.view(mesh.find_vertex(linestring.first()));
	if (!is_valid(*vertex_view))
		throw std::runtime_error("The first linestrip point is not a mesh vertex");

	vertex_fn(vertex_view);

	const Vertex_index first_vertex = *vertex_view;
	Vertex_index prev_vertex = Vertex_index::invalid;

	bool search_next;
	do
	{
		search_next = false;

		auto halfedge_circ = vertex_view.halfedge_in_circ();
		const auto halfedge_first = halfedge_circ;

		do
		{
			const auto vertex_from = halfedge_circ->vertex_index();
			if (vertex_from == prev_vertex || !contains(linestring, halfedge_circ->vertex()))
				continue;

			if (vertex_from == first_vertex)
				break;

			prev_vertex = *vertex_view;
			vertex_view.set_index(vertex_from);

			vertex_fn(vertex_view);
			halfedge_fn(*halfedge_circ);

			search_next = true;
			break;
		} while (++halfedge_circ != halfedge_first);
	} while (search_next);

	if (mesh.find_vertex(linestring.last()) != *vertex_view)
		throw std::runtime_error("The last linestrip point is not a mesh vertex");
}
} // namespace internal

inline std::vector<Vertex_index> vertices_in_linestrip(
	const Linestring& linestring, const Mesh2& mesh)
{
	std::vector<Vertex_index> vertices;

	enumerate_elements_in_linestrip(
		linestring, mesh, [&vertices](const auto& vertex) { vertices.push_back(*vertex); },
		[](const auto&) {});

	return vertices;
}

inline std::pair<std::vector<Vertex_index>, std::vector<Halfedge_index>>
vertices_and_halfedges_in_linestrip(const Linestring& linestring, const Mesh2& mesh)
{
	std::vector<Vertex_index> vertices;
	std::vector<Halfedge_index> halfedges;

	enumerate_elements_in_linestrip(
		linestring, mesh, [&vertices](const auto& vertex) { vertices.push_back(*vertex); },
		[&halfedges](const auto& halfedge) { halfedges.push_back(*halfedge); });

	return {vertices, halfedges};
}

inline std::vector<Halfedge_index> halfedges_in_linestrip(
	const Linestring& linestring, const Mesh2& mesh)
{
	std::vector<Halfedge_index> halfedges;

	enumerate_elements_in_linestrip(
		linestring, mesh, [](const auto&) {},
		[&halfedges](const auto& halfedge) { halfedges.push_back(*halfedge); });

	return halfedges;
}

inline std::pair<std::vector<Vertex_index>, std::vector<Edge_index>>
vertices_and_edges_in_linestrip(const Linestring& linestring, const Mesh2& mesh)
{
	std::vector<Vertex_index> vertices;
	std::vector<Edge_index> edges;

	enumerate_elements_in_linestrip(
		linestring, mesh, [&vertices](const auto& vertex) { vertices.push_back(*vertex); },
		[&edges](const auto& halfedge) { edges.push_back(halfedge.edge_index()); });

	return {vertices, edges};
}
} // namespace es_fe
