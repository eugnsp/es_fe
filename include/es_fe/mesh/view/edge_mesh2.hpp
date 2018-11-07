#pragma once
#include <es/fe/forward.hpp>
#include <es/fe/types.hpp>
#include <es/fe/mesh/view/base.hpp>

#include <es/geom/tags.hpp>
#include <es/geom/traits.hpp>
#include <es/geom/point.hpp>

namespace fe
{
template<>
class View<Edge_tag, Mesh2> : public internal::View_base<Edge_tag, Mesh2>
{
public:
	using View_base::View_base;
	using View_base::set_index;

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
};
}

namespace geom::internal
{
 template<>
 struct Traits<fe::View<fe::Edge_tag, fe::Mesh2>>
 {
 	using Index = fe::Local_index;
 	using Tag = Segment_tag;
 };
}