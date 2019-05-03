#pragma once
#include <es_fe/geometry/point1.hpp>
#include <es_fe/mesh/mesh1.hpp>
#include <es_fe/types.hpp>

#include <es_util/iterator.hpp>

#include <array>

namespace es_fe::internal
{
template<class Element>
class Boundary_cond_base<1, Element>
{
	static_assert(Element::dim == 1);
	static_assert(Element::has_vertex_dofs);

public:
	Boundary_cond_base(const Mesh<1>& mesh, es_fe::Point1 point)
	{
		vertex_[0] = mesh.find_vertex(point);
	}

	template<class... Args>
	auto vertices(const Args&...) const
	{
		static_assert(Element::has_vertex_dofs);
		return es_util::Iterable{vertex_};
	}

	template<class... Args>
	Vertex_index vertex(const Args&...) const
	{
		return vertex_[0];
	}

protected:
	std::array<Vertex_index, 1> vertex_;
};
} // namespace es_fe::internal
