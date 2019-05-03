#pragma once
#include <es_fe/geometry/linestring.hpp>
#include <es_fe/mesh/mesh2.hpp>
#include <es_fe/types.hpp>

#include <utility>
#include <vector>

namespace es_fe
{
std::vector<Vertex_index> vertices_in_linestring(const Mesh2&, const Linestring&);
std::vector<Halfedge_index> halfedges_in_linestring(const Mesh2&, const Linestring&);

std::pair<std::vector<Vertex_index>, std::vector<Halfedge_index>>
vertices_and_halfedges_in_linestring(const Mesh2&, const Linestring&);

std::pair<std::vector<Vertex_index>, std::vector<Edge_index>> vertices_and_edges_in_linestring(
	const Mesh2&, const Linestring&);
} // namespace es_fe
