#pragma once
#include <es_fe/types.hpp>
#include <es_fe/mesh/view/base.hpp>

#include <es_geom/tags.hpp>
#include <es_geom/traits.hpp>
#include <es_geom/point.hpp>

namespace es_fe
{
template<>
class Element_view<Edge_tag, Mesh2> : public internal::Element_view_base<Edge_tag, Mesh2>
{
public:
	using Element_view_base::set_index;
	using Element_view_base::Element_view_base;

	Edge_index operator*() const
	{
		return edge(this->index_);
	}

	void set_index(Edge_index index)
	{
		set_index(first_halfedge(index));
	}

	bool is_boundary() const
	{
		return this->mesh_.is_boundary(**this);
	}

	const geom::Point& vertex(Local_index) const;
	Vertex_index vertex_index(Local_index) const;

	void print(std::ostream&) const;
};

inline std::ostream& operator<<(std::ostream& os, const Element_view<Edge_tag, Mesh2>& view)
{
	view.print(os);
	return os;
}
} // namespace es_fe

namespace geom::internal
{
template<>
struct Traits<es_fe::Element_view<es_fe::Edge_tag, es_fe::Mesh2>>
{
	using Index = es_fe::Local_index;
	using Tag = Segment_tag;
};
} // namespace geom::internal