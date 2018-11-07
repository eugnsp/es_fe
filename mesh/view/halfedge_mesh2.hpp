#pragma once
#include <es/fe/forward.hpp>
#include <es/fe/types.hpp>
#include <es/fe/mesh/view/base.hpp>

#include <es/geom/point.hpp>

namespace fe
{

template<>
class View<Halfedge_tag, Mesh2> : public internal::View_base<Halfedge_tag, Mesh2>
{
// 	friend class Circular_iterator<Edge_tag, Vertex_out_circ_tag>;
// 	friend class Circular_iterator<Edge_tag, Face_circ_tag>;

public:
	using Vertex_view = View<Vertex_tag, Mesh2>;
	using Face_view = View<Face_tag, Mesh2>;

public:
	using View_base::View_base;

	const geom::Point& vertex() const;
	const geom::Point& vertex(Local_index) const;
	
	geom::Point vector() const;

	Vertex_index vertex_index() const;
	Vertex_index vertex_index(Local_index) const;

	Edge_index edge_index() const;

	Vertex_view vertex_view() const;
	
// 	Index half_edge_index() const;
// 
// 	static constexpr Local_index n_vertices()
// 	{
// 		return 2;
// 	}
// 
// 	// The edge is directed from local index 0 to index 1
// 	Index vertex_index(Local_index) const;
// 	void vertex_indices(std::array<Index, 2>&) const;

// 	Vertex_index vertex_from_index() const
// 	{
// 		throw;
// 	}			// = vertex_index(0)
// 	Vertex_index vertex_to_index() const
// 	{
// 		throw;
// 	}				// = vertex_index(1)
// 
// 	const geom::Point& vertex(Local_index) const
// 	{
// 		throw;
// 	}
// 
// 	template<Local_index index>
// 	const geom::Point& vertex() const
// 	{
// 		static_assert(index < 2, "Index out of bounds");
// 		return vertex(index);
// 	}
// 
// 	const geom::Point& vertex_from() const
// 	{
// 		return vertex<0>();
// 	}
// 
// 	const geom::Point& vertex_to() const
// 	{
// 		return vertex<1>();
// 	}
// 
	//Vertex_view vertex_view(Local_index) const;
// 	Vertex_view vertex_from_view() const
// 	{
// 		throw;
// 	}		// = vertex_view(0)
// 	Vertex_view vertex_to_view() const
// 	{
// 		throw;
// 	}			// = vertex_view(1)
// 

	Face_index face_index() const;
	Face_index twin_face_index() const;
	Face_index face_index(Local_index) const;	// The face on the left (right) has local index 0 (1)
 
 	Face_view face_view() const;
 	Face_view twin_face_view() const;
 	Face_view face_view(Local_index) const;		// The face on the left (right) has local index 0 (1)
 
// 	// Index oppositeVertexIndex(Local_index) const;
// 
// 	// Segment concept
// 	// TODO : rename??
// 	const geom::Point& first() const
// 	{
// 		return vertex_from();
// 	}
// 
// 	const geom::Point& second() const
// 	{
// 		return vertex_to();
// 	}
};
}

namespace geom::internal
{
 template<>
 struct Traits<fe::View<fe::Halfedge_tag, fe::Mesh2>>
 {
 	using Index = fe::Local_index;
 	using Tag = Segment_tag;
 };
}