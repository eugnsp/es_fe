#pragma once
#include <es_fe/geom/linestring.hpp>
#include <es_fe/mesh/algorithm/elements_in_linestring.hpp>
#include <es_fe/mesh/mesh2.hpp>
#include <es_fe/types.hpp>

#include <es_util/iterator.hpp>

#include <tuple>
#include <vector>

namespace es_fe
{
template<class Element, bool is_essential_>
class Boundary_cond
{
public:
	static constexpr bool is_essential = is_essential_;

public:
	Boundary_cond(const Mesh2& mesh, const Linestring& boundary)
	{
		static_assert(Element::has_vertex_dofs || Element::has_edge_dofs);
		
		if constexpr (Element::has_vertex_dofs && Element::has_edge_dofs)
			std::tie(vertices_, halfedges_) = vertices_and_halfedges_in_linestring(mesh, boundary);
		else if constexpr (Element::has_vertex_dofs)
			vertices_ = vertices_in_linestring(mesh, boundary);
		else
			halfedges_ = halfedges_in_linestring(mesh, boundary);
	}
	
	auto vertices() const
	{
		static_assert(Element::has_vertex_dofs);
		return es_util::Iterable{vertices_};
	}

	auto halfedges() const
	{
		static_assert(Element::has_edge_dofs);
		return es_util::Iterable{halfedges_};
	}
	
protected:
	std::vector<Vertex_index> vertices_;
	std::vector<Halfedge_index> halfedges_;
};
}
