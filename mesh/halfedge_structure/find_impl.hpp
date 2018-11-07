#pragma once
#include <es/fe/mesh/halfedge_structure/halfedge_structure.hpp>

namespace fe::internal
{
inline auto Halfedge_structure::find_vertex(const geom::Point& point) const -> Vertex_index
{
	const auto pos = std::find_if(vertices_.begin(), vertices_.end(),
		[&point](auto& vertex) { return vertex.point == point; });

	if (pos != vertices_.end())
		return Vertex_index{static_cast<Index>(pos - vertices_.begin())};
	else
		return Vertex_index::invalid;
}

inline auto Halfedge_structure::find_halfedge(
	Vertex_index vertex_from, Vertex_index vertex_to) const -> Halfedge_index
{
	const auto halfedge = vertices_[*vertex_to].halfedge;
	if (!is_valid(halfedge))
		return Halfedge_index::invalid;

	return find(twin(halfedge),
		[vertex_from, this](Halfedge_index edge)
		{
			return (!is_valid(edge) || halfedges_[*edge].vertex == vertex_from);
		}, Vertex_in_circ_tag{});
}
}