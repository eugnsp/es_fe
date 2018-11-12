#include <es_fe/types.hpp>
#include <es_fe/mesh/mesh2.hpp>

namespace es_fe
{
auto Mesh2::begin_vertex() const -> Vertex_iter
{
	return {*this, Vertex_index{0}};
}

auto Mesh2::end_vertex() const -> Vertex_iter
{
	return {*this, n_vertices()};
}

auto Mesh2::vertices() const -> es_util::Iterable<Vertex_iter>
{
	return {begin_vertex(), end_vertex()};
}

auto Mesh2::begin_halfedge() const -> Halfedge_iter
{
	return {*this, Halfedge_index{0}};
}

auto Mesh2::end_halfedge() const -> Halfedge_iter
{
	return {*this, n_halfedges()};
}

auto Mesh2::halfedges() const -> es_util::Iterable<Halfedge_iter>
{
	return {begin_halfedge(), end_halfedge()};
}

auto Mesh2::begin_edge() const -> Edge_iter
{
	return {*this, Halfedge_index{0}};
}

auto Mesh2::end_edge() const -> Edge_iter
{
	return {*this, n_halfedges()};
}

// auto Mesh2::edges() const -> es::util::Iterable<Edge_iter>
// {
// 	return {begin_edge(), end_edge()};
// }

auto Mesh2::begin_face() const -> Face_iter
{
	return {*this, Face_index{0}};
}

auto Mesh2::end_face() const -> Face_iter
{
	return {*this, n_faces()};
}

auto Mesh2::faces() const -> es_util::Iterable<Face_iter>
{
	return {begin_face(), end_face()};
}

auto Mesh2::begin_cell() const -> Cell_iter
{
	return begin_face();
}

auto Mesh2::end_cell() const -> Cell_iter
{
	return end_face();
}

auto Mesh2::cells() const -> es_util::Iterable<Cell_iter>
{
	return faces();
}
} // namespace es_fe