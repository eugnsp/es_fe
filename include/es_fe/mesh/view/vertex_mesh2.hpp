#pragma once
#include <es_fe/types.hpp>
#include <es_fe/type_traits.hpp>
#include <es_fe/mesh/view/base.hpp>

#include <es_geom/point.hpp>

namespace es_fe
{
template<>
class Element_view<Vertex_tag, Mesh2> : public internal::Element_view_base<Vertex_tag, Mesh2>
{
public:
	using Vertex_circ = Circulator<Vertex_tag, Vertex_out_circ_tag>;

	using Halfedge_in_circ = Circulator<Halfedge_tag, Vertex_in_circ_tag>;
	using Halfedge_out_circ = Circulator<Halfedge_tag, Vertex_out_circ_tag>;

	using Edge_circ = Circulator<Edge_tag, Vertex_out_circ_tag>;
	using Face_circ = Circulator<Face_tag, Vertex_out_circ_tag>;

public:
	using Element_view_base::Element_view_base;

	const geom::Point& vertex() const;

	Vertex_circ vertex_circ() const;
	Halfedge_in_circ halfedge_in_circ() const;
	Halfedge_out_circ halfedge_out_circ() const;
	Edge_circ edge_circ() const;
	Face_circ face_circ() const;

	Local_index n_edges() const;
	Local_index n_faces() const;
};
} // namespace es_fe