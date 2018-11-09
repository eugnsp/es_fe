#pragma once
#include <es_fe/mesh/mesh2/mesh2.hpp>

#include <es_fe/types.hpp>
#include <es_fe/mesh/view/vertex_mesh2.hpp>
#include <es_fe/mesh/view/halfedge_mesh2.hpp>
#include <es_fe/mesh/view/edge_mesh2.hpp>
#include <es_fe/mesh/view/face_mesh2.hpp>
#include <es_fe/mesh/iterator/random_access.hpp>
//#include <es_fe/mesh/iterator/circulator_vertex.hpp>
//#include <es_fe/mesh/iterator/circulator_halfedge.hpp>
//#include <es_fe/mesh/iterator/circulator_edge.hpp>
//#include <es_fe/mesh/iterator/circulator_face.hpp>

#include <es_util/function.hpp>

namespace es_fe
{
//////////////////////////////////////////////////////////////////////////
/** Views */

inline auto Mesh2::view(Vertex_index index) const -> Vertex_view
{
	return {*this, index};
}

inline auto Mesh2::view(Halfedge_index index) const -> Halfedge_view
{
	return {*this, index};
}

inline auto Mesh2::view(Edge_index index) const -> Edge_view
{
	return {*this, first_halfedge(index)};
}

inline auto Mesh2::view(Face_index index) const -> Face_view
{
	return {*this, index};
}

//////////////////////////////////////////////////////////////////////////
/** Iterators */

inline auto Mesh2::begin_vertex() const -> Vertex_iter
{
	return {*this, Vertex_index{0}};
}

inline auto Mesh2::end_vertex() const -> Vertex_iter
{
	return {*this, n_vertices()};
}

inline auto Mesh2::vertices() const -> es_util::Iterable<Vertex_iter>
{
	return {begin_vertex(), end_vertex()};
}

inline auto Mesh2::begin_halfedge() const -> Halfedge_iter
{
	return {*this, Halfedge_index{0}};
}

inline auto Mesh2::end_halfedge() const -> Halfedge_iter
{
	return {*this, n_halfedges()};
}

inline auto Mesh2::halfedges() const -> es_util::Iterable<Halfedge_iter>
{
	return {begin_halfedge(), end_halfedge()};
}

inline auto Mesh2::begin_edge() const -> Edge_iter
{
	return {*this, Halfedge_index{0}};
}

inline auto Mesh2::end_edge() const -> Edge_iter
{
	return {*this, n_halfedges()};
}

// inline auto Mesh2::edges() const -> es::util::Iterable<Edge_iter>
// {
// 	return {begin_edge(), end_edge()};
// }

inline auto Mesh2::begin_face() const -> Face_iter
{
	return {*this, Face_index{0}};
}

inline auto Mesh2::end_face() const -> Face_iter
{
	return {*this, n_faces()};
}

inline auto Mesh2::faces() const -> es_util::Iterable<Face_iter>
{
	return {begin_face(), end_face()};
}

inline auto Mesh2::begin_cell() const -> Cell_iter
{
	return begin_face();
}

inline auto Mesh2::end_cell() const -> Cell_iter
{
	return end_face();
}

inline auto Mesh2::cells() const -> es_util::Iterable<Cell_iter>
{
	return faces();
}

inline std::ostream& operator<<(std::ostream& ost, const Mesh2& mesh)
{
	ost << mesh.type_string() << '\n'
		<< "Vertices: " << *mesh.n_vertices() << '\n'
		<< "Edges:    " << *mesh.n_edges() << '\n'
		<< "Faces:    " << *mesh.n_faces() << '\n'
		<< "Memory:   " << es_util::size_string(mesh.memory_size()) << std::endl;

	return ost;
}
} // namespace es_fe