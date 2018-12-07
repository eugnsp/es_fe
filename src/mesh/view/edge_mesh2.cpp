#include <es_fe/types.hpp>
#include <es_fe/mesh/mesh2.hpp>

#include <es_geom/point.hpp>

#include <cassert>

namespace es_fe
{
bool Element_view<Edge_tag, Mesh2>::is_boundary() const
{
	return this->mesh_.is_boundary(**this);
}

const es_geom::Point& Element_view<Edge_tag, Mesh2>::vertex(Local_index index) const
{
	return mesh_.vertex(vertex_index(index));
}

Vertex_index Element_view<Edge_tag, Mesh2>::vertex_index(Local_index index) const
{
	assert(index < 2);
	if (index == 0)
		return mesh_.vertex_index(this->index_);
	else
		return mesh_.vertex_index(twin(this->index_));
}
} // namespace es_fe