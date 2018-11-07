#pragma once
#include <cassert>

// TODO : includes

namespace fe
{
inline const geom::Point& View<Halfedge_tag, Mesh2>::vertex() const
{
	return mesh_.vertex(vertex_index());
}

inline const geom::Point& View<Halfedge_tag, Mesh2>::vertex(Local_index vertex) const
{
	return mesh_.vertex(vertex_index(vertex));
}

inline Vertex_index View<Halfedge_tag, Mesh2>::vertex_index() const
{
	return mesh_.vertex_index(index_);
}

inline Vertex_index View<Halfedge_tag, Mesh2>::vertex_index(Local_index vertex) const
{
	assert(vertex < 2);
	if (vertex == 0)
		return mesh_.vertex_index(index_);
	else
		return mesh_.vertex_index(twin(index_));
}

inline geom::Point View<Halfedge_tag, Mesh2>::vector() const
{
	return vertex() - mesh_.vertex(mesh_.vertex_index(twin(index_)));
}

inline Edge_index View<Halfedge_tag, Mesh2>::edge_index() const
{
	return edge(index_);
}

inline auto View<Halfedge_tag, Mesh2>::vertex_view() const -> Vertex_view
{
	return {mesh_, vertex_index()};
}

inline Face_index View<Halfedge_tag, Mesh2>::face_index() const
{
	return mesh_.face_index(index_);
}

inline Face_index View<Halfedge_tag, Mesh2>::twin_face_index() const
{
	return mesh_.face_index(twin(index_));
}

inline Face_index View<Halfedge_tag, Mesh2>::face_index(Local_index index) const
{
	assert(index < 2);
	if (index == 0)
		return face_index();
	else
		return twin_face_index();
}

inline auto View<Halfedge_tag, Mesh2>::face_view() const -> Face_view
{
	return {mesh_, face_index()};
}

inline auto View<Halfedge_tag, Mesh2>::twin_face_view() const -> Face_view
{
	return {mesh_, twin_face_index()};
}

inline auto View<Halfedge_tag, Mesh2>::face_view(Local_index index) const -> Face_view
{
	return {mesh_, face_index(index)};
}
}