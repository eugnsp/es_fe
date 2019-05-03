#include <es_fe/mesh/mesh1.hpp>
#include <es_fe/types.hpp>

namespace es_fe
{
auto Mesh1::begin_vertex() const -> Vertex_iter
{
	return {*this, Vertex_index{0}};
}

auto Mesh1::end_vertex() const -> Vertex_iter
{
	return {*this, n_vertices()};
}

auto Mesh1::vertices() const -> es_util::Iterable<Vertex_iter>
{
	return {begin_vertex(), end_vertex()};
}

auto Mesh1::begin_edge() const -> Edge_iter
{
	return {*this, Edge_index{0}};
}

auto Mesh1::end_edge() const -> Edge_iter
{
	return {*this, n_edges()};
}

auto Mesh1::edges() const -> es_util::Iterable<Edge_iter>
{
	return {begin_edge(), end_edge()};
}

auto Mesh1::begin_cell() const -> Cell_iter
{
	return begin_edge();
}

auto Mesh1::end_cell() const -> Cell_iter
{
	return end_edge();
}

auto Mesh1::cells() const -> es_util::Iterable<Cell_iter>
{
	return edges();
}
} // namespace es_fe
