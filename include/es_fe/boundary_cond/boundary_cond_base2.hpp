#pragma once
#include <es_fe/geometry/linestring.hpp>
#include <es_fe/mesh/algorithm/linestring.hpp>
#include <es_fe/mesh/mesh2.hpp>
#include <es_fe/types.hpp>

#include <es_util/iterator.hpp>

#include <tuple>
#include <vector>

namespace es_fe::internal
{
template<class Element>
class Boundary_cond_base<2, Element>
{
	static_assert(Element::dim == 2);
	static_assert(Element::has_vertex_dofs || Element::has_edge_dofs);

public:
	Boundary_cond_base(const Mesh<2>& mesh, const Linestring& boundary)
	{
		if constexpr (Element::has_vertex_dofs && Element::has_edge_dofs)
			std::tie(vertices_, halfedges_) = vertices_and_halfedges_in_linestring(mesh, boundary);
		else if constexpr (Element::has_vertex_dofs)
			vertices_ = vertices_in_linestring(mesh, boundary);
		else
			halfedges_ = halfedges_in_linestring(mesh, boundary);
	}

	template<class... Args>
	auto vertices(const Args&...) const
	{
		static_assert(Element::has_vertex_dofs);
		return es_util::Iterable{vertices_};
	}

	template<class... Args>
	auto halfedges(const Args&...) const
	{
		static_assert(Element::has_edge_dofs);
		return es_util::Iterable{halfedges_};
	}

protected:
	std::vector<Vertex_index> vertices_;
	std::vector<Halfedge_index> halfedges_;
};
} // namespace es_fe::internal
