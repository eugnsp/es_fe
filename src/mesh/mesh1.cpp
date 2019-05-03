#include <es_fe/geometry.hpp>
#include <es_fe/mesh/mesh1.hpp>

#include <algorithm>

namespace es_fe
{
es_fe::Vertex_index Mesh1::find_vertex(es_fe::Point1 point) const
{
	const auto pos = std::lower_bound(vertices_.begin(), vertices_.end(), point);
	if (pos != vertices_.end() && *pos == point)
		return static_cast<es_fe::Vertex_index>(pos - vertices_.begin());
	else
		return es_fe::Vertex_index::invalid;
}

bool Mesh1::is_boundary(es_fe::Vertex_index index) const
{
	return *index == 0 || index == (n_vertices() - 1);
}

bool Mesh1::is_boundary(es_fe::Edge_index index) const
{
	return *index == 0 || index == (n_edges() - 1);
}

std::pair<es_fe::Vertex_index, es_fe::Vertex_index> Mesh1::vertex_indices(
	es_fe::Edge_index edge) const
{
	const auto first_vertex = es_fe::Vertex_index{*edge};
	return {first_vertex, first_vertex + 1};
}
} // namespace es_fe
