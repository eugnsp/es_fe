#pragma once
#include <cassert>

namespace fe
{
inline const geom::Point& View<Edge_tag, Mesh2>::vertex(Local_index index) const
{
	return mesh_.vertex(vertex_index(index));
}

inline Vertex_index View<Edge_tag, Mesh2>::vertex_index(Local_index index) const
{
	assert(index < 2);
	if (index == 0)
		return mesh_.vertex_index(this->index_);
	else
		return mesh_.vertex_index(twin(this->index_));
}
}