#pragma once
#include <es_fe/mesh/view/face_mesh2.hpp>

#include <es_fe/mesh/mesh2.hpp>
#include <es_fe/mesh/iterator/circulator_vertex_face.hpp>
#include <es_fe/mesh/iterator/circulator_halfedge_edge.hpp>
#include <es_fe/types.hpp>

//#include <es_geom/point.hpp>
#include <array>
#include <cassert>
#include <utility>

namespace es_fe
{
inline Halfedge_index Element_view<Face_tag, Mesh2>::halfedge() const
{
	return mesh_.halfedge_index(index_);
}

inline auto Element_view<Face_tag, Mesh2>::vertex_circ() const -> Vertex_circ
{
	return Vertex_circ{mesh_, halfedge()};
}

inline auto Element_view<Face_tag, Mesh2>::halfedge_circ() const -> Halfedge_circ
{
	return Halfedge_circ{mesh_, halfedge()};
}

inline auto Element_view<Face_tag, Mesh2>::edge_circ() const -> Edge_circ
{
	return Edge_circ{mesh_, halfedge()};
}

inline auto Element_view<Face_tag, Mesh2>::face_circ() const -> Face_circ
{
	return Face_circ{mesh_, halfedge()};
}

inline void Element_view<Face_tag, Mesh2>::get_indices(Vertex_indices& vertices) const
{
	auto halfedge = halfedge_circ();
	vertices[0] = halfedge->vertex_index();
	vertices[1] = (++halfedge)->vertex_index();
	vertices[2] = (++halfedge)->vertex_index();

	assert(++halfedge == halfedge_circ());
}

inline void Element_view<Face_tag, Mesh2>::get_indices(Vertex_indices& vertices,
											   Halfedge_indices& halfedges) const
{
	auto halfedge = halfedge_circ();
	vertices[0] = halfedge->vertex_index();
	halfedges[0] = **halfedge;
	vertices[1] = (++halfedge)->vertex_index();
	halfedges[1] = **halfedge;
	vertices[2] = (++halfedge)->vertex_index();
	halfedges[2] = **halfedge;

	assert(++halfedge == halfedge_circ());
}

inline void Element_view<Face_tag, Mesh2>::get_indices(Vertex_indices& vertices,
											   Edge_indices& edges) const
{
	auto halfedge = halfedge_circ();
	vertices[0] = halfedge->vertex_index();
	edges[0] = edge(**halfedge);
	vertices[1] = (++halfedge)->vertex_index();
	edges[1] = edge(**halfedge);
	vertices[2] = (++halfedge)->vertex_index();
	edges[2] = edge(**halfedge);

	assert(++halfedge == halfedge_circ());
}

inline void Element_view<Face_tag, Mesh2>::get_indices(
	Vertex_indices& vertices, Edge_with_dir_indices& edges_with_direction) const
{
	auto halfedge = halfedge_circ();
	vertices[0] = halfedge->vertex_index();
	edges_with_direction[0] = {edge(**halfedge), is_first_halfedge(**halfedge)};
	vertices[1] = (++halfedge)->vertex_index();
	edges_with_direction[1] = {edge(**halfedge), is_first_halfedge(**halfedge)};
	vertices[2] = (++halfedge)->vertex_index();
	edges_with_direction[2] = {edge(**halfedge), is_first_halfedge(**halfedge)};

	assert(++halfedge == halfedge_circ());
}

inline void Element_view<Face_tag, Mesh2>::get_indices(Halfedge_index first_halfedge,
											   Vertex_indices& vertices,
											   Halfedge_indices& halfedges) const
{
	assert(mesh_.face_index(first_halfedge) == index_);

	auto halfedge = Halfedge_circ{mesh_, first_halfedge};
	vertices[0] = halfedge->vertex_index();
	halfedges[0] = **halfedge;
	vertices[1] = (++halfedge)->vertex_index();
	halfedges[1] = **halfedge;
	vertices[2] = (++halfedge)->vertex_index();
	halfedges[2] = **halfedge;

	assert(**(++halfedge) == first_halfedge);
}
} // namespace es_fe