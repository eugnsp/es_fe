#include <es_fe/types.hpp>
#include <es_fe/mesh/mesh2.hpp>

namespace es_fe
{
auto Mesh2::view(Vertex_index index) const -> Vertex_view
{
	// TODO : assert index is valid
	return {*this, index};
}

auto Mesh2::view(Halfedge_index index) const -> Halfedge_view
{
	return {*this, index};
}

auto Mesh2::view(Edge_index index) const -> Edge_view
{
	return {*this, first_halfedge(index)};
}

auto Mesh2::view(Face_index index) const -> Face_view
{
	return {*this, index};
}
} // namespace es_fe