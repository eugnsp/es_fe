#pragma once
#include <es_fe/mesh/halfedge_structure/halfedge_structure.hpp>

namespace es_fe::internal
{
inline bool Halfedge_structure::is_boundary(Vertex_index index) const
{
	const auto halfedge = vertices_[*index].halfedge;
	if (!is_valid(halfedge))
		return true;

	// For boundary vertices, the half-edge is always the outer boundary half-edge,
	// so we just check the corresponding cell index of that edge
	return !is_valid(halfedges_[*halfedge].face);
}

inline bool Halfedge_structure::is_boundary(Edge_index index) const
{
	return is_boundary(first_halfedge(index)) || is_boundary(second_halfedge(index));
}

inline bool Halfedge_structure::is_boundary(Halfedge_index index) const
{
	return !is_valid(halfedges_[*index].face);
}

inline bool Halfedge_structure::is_boundary(Face_index index) const
{
	const auto boundary_twin_halfedge =
		find(faces_[*index].halfedge,
			 [this](Halfedge_index edge) { return is_boundary(twin(edge)); }, Face_circ_tag{});

	return is_valid(boundary_twin_halfedge);
}
} // namespace es_fe::internal