#pragma once
#include <es_fe/geom/point.hpp>
#include <es_fe/mesh/view/base.hpp>
#include <es_fe/types.hpp>

namespace es_fe
{
template<>
class Element_view<Edge_tag, Mesh2> : public internal::Element_view_base<Edge_tag, Mesh2>
{
public:
	using Geometry_tag = Segment_tag;

public:
	using Element_view_base::Element_view_base;
	using Element_view_base::set_index;

	Edge_index operator*() const
	{
		return edge(this->index_);
	}

	void set_index(Edge_index index)
	{
		set_index(first_halfedge(index));
	}

	bool is_boundary() const;

	const Point& vertex(Local_index) const;
	Vertex_index vertex_index(Local_index) const;

	// Outputs human readable information about the view
	void print(std::ostream&) const;
};

// Outputs human readable information about the view
inline std::ostream& operator<<(std::ostream& os, const Element_view<Edge_tag, Mesh2>& view)
{
	view.print(os);
	return os;
}
} // namespace es_fe
