#pragma once
#include <es_fe/mesh/base.hpp>
#include <es_fe/mesh/halfedge_structure.hpp>
#include <es_fe/mesh/view/base.hpp>

#include <es_geom/tags.hpp>
#include <es_geom/traits.hpp>
#include <es_geom/point.hpp>
#include <es_util/iterator.hpp>

#include <cstddef>
#include <utility>
#include <array>
#include <vector>
#include <iosfwd>
#include <cassert>

namespace es_fe
{
//////////////////////////////////////////////////////////////////////////
/** 1D mesh views */

// template<>
// class Element_view<Vertex_tag, Mesh1> : public internal::Element_view_base<Vertex_tag, Mesh1>
// {
// public:
// 	using Element_view_base::Element_view_base;
//
// 	const geom::Point& vertex() const;
//
// 	Element_view<Edge_tag, Mesh1> _first_cell() const;
// };
//
// template<>
// class Element_view<Edge_tag, Mesh1> : public internal::Element_view_base<Edge_tag, Mesh1>
// {
// public:
// 	using Element_view_base::Element_view_base;
//
// 	Index vertex_index(Local_index) const;
// 	void vertex_indices(Index_array<2>&) const;
//
// 	const geom::Point& vertex(Local_index) const;
// };

//////////////////////////////////////////////////////////////////////////
/** 2D mesh views */

template<>
class Element_view<Halfedge_tag, Mesh2> : public internal::Element_view_base<Halfedge_tag, Mesh2>
{
	// 	friend class Circular_iterator<Edge_tag, Vertex_out_circ_tag>;
	// 	friend class Circular_iterator<Edge_tag, Face_circ_tag>;

public:
	using Vertex_view = Element_view<Vertex_tag, Mesh2>;
	using Face_view = Element_view<Face_tag, Mesh2>;

public:
	using Element_view_base::Element_view_base;

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

	Vertex_index vertex_from_index() const
	{
		throw;
	} // = vertex_index(0)
	Vertex_index vertex_to_index() const
	{
		throw;
	} // = vertex_index(1)

	const geom::Point& vertex(Local_index) const
	{
		throw;
	}

	template<Local_index index>
	const geom::Point& vertex() const
	{
		static_assert(index < 2, "Index out of bounds");
		return vertex(index);
	}

	const geom::Point& vertex_from() const
	{
		return vertex<0>();
	}

	const geom::Point& vertex_to() const
	{
		return vertex<1>();
	}

	// Vertex_view vertex_view(Local_index) const;
	Vertex_view vertex_from_view() const
	{
		throw;
	} // = vertex_view(0)
	Vertex_view vertex_to_view() const
	{
		throw;
	} // = vertex_view(1)

	// 	// The face on the left (right) has local index 0 (1)
	// 	Index face_index(Local_index) const;
	// 	Index left_face_index() const;				// = face_index(0)
	// 	Index right_face_index() const;				// = face_index(1)
	//
	// 	Face_view face_view(Local_index) const;
	// 	Face_view left_face_view() const;			// = face_view(0)
	// 	Face_view right_face_view() const;			// = face_view(1)
	//
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

// Edge view (edge view has an arbitrary, but fixed, direction)
template<>
class Element_view<Edge_tag, Mesh2> : public internal::Element_view_base<Edge_tag, Mesh2>
{
	friend class Circular_iterator<Edge_tag, Vertex_out_circ_tag>;
	friend class Circular_iterator<Edge_tag, Face_circ_tag>;

public:
	using Vertex_view = Element_view<Vertex_tag, Mesh2>;
	using Face_view = Element_view<Face_tag, Mesh2>;

public:
	using Element_view_base::Element_view_base;

	Index half_edge_index() const;

	static constexpr Local_index n_vertices()
	{
		return 2;
	}

	// The edge is directed from local index 0 to index 1
	Index vertex_index(Local_index) const;
	void vertex_indices(std::array<Index, 2>&) const;

	Vertex_index vertex_from_index() const
	{
		throw;
	} // = vertex_index(0)
	Vertex_index vertex_to_index() const
	{
		throw;
	} // = vertex_index(1)

	const geom::Point& vertex(Local_index) const;

	template<Local_index index>
	const geom::Point& vertex() const
	{
		static_assert(index < 2, "Index out of bounds");
		return vertex(index);
	}

	const geom::Point& vertex_from() const
	{
		return vertex<0>();
	}

	const geom::Point& vertex_to() const
	{
		return vertex<1>();
	}

	Vertex_view vertex_view(Local_index) const;
	Vertex_view vertex_from_view() const; // = vertex_view(0)
	Vertex_view vertex_to_view() const;   // = vertex_view(1)

	// The face on the left (right) has local index 0 (1)
	Index face_index(Local_index) const;
	Index left_face_index() const;  // = face_index(0)
	Index right_face_index() const; // = face_index(1)

	Face_view face_view(Local_index) const;
	Face_view left_face_view() const;  // = face_view(0)
	Face_view right_face_view() const; // = face_view(1)

	// Index oppositeVertexIndex(Local_index) const;

	// Segment concept
	// TODO : rename??
	const geom::Point& first() const
	{
		return vertex_from();
	}

	const geom::Point& second() const
	{
		return vertex_to();
	}
};

// Cell view
template<>
class Element_view<Face_tag, Mesh2> : public internal::Element_view_base<Face_tag, Mesh2>
{
public:
	// TODO : dynamic ?
#ifdef TRIANGULAR_CELLS_ONLY
	using Vertex_indices = std::array<Vertex_index, 3>;
	using Edge_indices = std::array<Edge_index, 3>;
	using Directed_edge_indices = std::array<std::pair<Edge_index, bool>, 3>;
#endif

private:
	using Vertex_iter = Circular_iterator<Vertex_tag, Face_circ_tag>;
	using Edge_iter = Circular_iterator<Edge_tag, Face_circ_tag>;
	using Face_iter = Circular_iterator<Face_tag, Face_circ_tag>;

	using Vertex_end_iter = End_circular_iterator<Vertex_tag, Face_circ_tag>;
	using Edge_end_iter = End_circular_iterator<Edge_tag, Face_circ_tag>;
	using Face_end_iter = End_circular_iterator<Face_tag, Face_circ_tag>;

public:
	using Element_view_base::Element_view_base;

#ifndef TRIANGULAR_CELLS_ONLY
	Local_index n_vertices() const;
#else
	static constexpr Local_index n_vertices()
	{
		return 3;
	}
#endif

	Vertex_index vertex_index(Local_index) const;
	void vertex_indices(std::vector<Vertex_index>&) const;
	void vertex_and_edge_indices(std::vector<Vertex_index>& vertices,
								 std::vector<Edge_index>& edges) const;

	// edges[i].second is true (false) if the corresponding edge view
	// has this face on the left (right)
	void vertex_and_edge_indices(std::vector<Vertex_index>& vertices,
								 std::vector<std::pair<Edge_index, bool>>& edges) const;

#ifdef TRIANGULAR_CELLS_ONLY
	void vertex_indices(Vertex_indices&) const;
	void edge_indices(Edge_indices&) const;
	void vertex_and_edge_indices(Vertex_indices& vertices, Edge_indices& edges) const;

	// edges[i].second is true (false) if the corresponding edge view
	// has this face on the left (right)
	void vertex_and_edge_indices(Vertex_indices& vertices, Directed_edge_indices& edges) const;
#endif

	/** Returns a vertex by the local index of the vertex in the cell */
	const geom::Point& vertex(Local_index index) const;

#ifdef TRIANGULAR_CELLS_ONLY
	template<Local_index index>
	const geom::Point& vertex() const
	{
		static_assert(index < 3, "Index out of bounds");
		return vertex(index);
	}
#endif

	/** CCW iterators */

	Vertex_iter begin_vertex() const;
	Vertex_end_iter end_vertex() const;

	Edge_iter begin_edge() const;
	Edge_end_iter end_edge() const;

	Face_iter begin_face() const;
	Face_end_iter end_face() const;

	es::util::Iterable<Vertex_iter, Vertex_end_iter> vertices() const;
	es::util::Iterable<Edge_iter, Edge_end_iter> edges() const;
	es::util::Iterable<Face_iter, Face_end_iter> faces() const;

private:
	Halfedge_index bounding_halfedge() const;
};

// template<>
// class Element_view2<Face_tag, Mesh2> : public internal::Element_view_base2<Face_tag, Mesh2>
// {
// public:
// 	// TODO : dynamic ?
// #ifdef TRIANGULAR_CELLS_ONLY
// 	using Vertex_indices = Index_array<3>;
// 	using Edge_indices = Index_array<3>;
// 	using Directed_edge_indices = std::array<std::pair<Index, bool>, 3>;
// #endif
//
// private:
// 	using Vertex_iter = Circular_iterator<Vertex_tag, Face_circ_tag>;
// 	using Edge_iter = Circular_iterator<Edge_tag, Face_circ_tag>;
// 	using Face_iter = Circular_iterator<Face_tag, Face_circ_tag>;
//
// 	using Vertex_end_iter = End_circular_iterator<Vertex_tag, Face_circ_tag>;
// 	using Edge_end_iter = End_circular_iterator<Edge_tag, Face_circ_tag>;
// 	using Face_end_iter = End_circular_iterator<Face_tag, Face_circ_tag>;
//
// public:
// 	using Element_view_base2::Element_view_base2;
//
// #ifndef TRIANGULAR_CELLS_ONLY
// 	Local_index n_vertices() const;
// #else
// 	static constexpr Local_index n_vertices()
// 	{
// 		return 3;
// 	}
// #endif
//
// 	Index vertex_index(Local_index) const;
// 	void vertex_indices(std::vector<Index>&) const;
// 	void vertex_and_edge_indices(std::vector<Index>& vertices, std::vector<Index>& edges) const;
//
// 	// edges[i].second is true (false) if the corresponding edge view
// 	// has this face on the left (right)
// 	void vertex_and_edge_indices(std::vector<Index>& vertices,
// 		std::vector<std::pair<Index, bool>>& edges) const;
//
// #ifdef TRIANGULAR_CELLS_ONLY
// 	void vertex_indices(Vertex_indices&) const;
// 	void edge_indices(Edge_indices&) const;
// 	void vertex_and_edge_indices(Vertex_indices& vertices, Edge_indices& edges) const;
//
// 	// edges[i].second is true (false) if the corresponding edge view
// 	// has this face on the left (right)
// 	void vertex_and_edge_indices(Vertex_indices& vertices, Directed_edge_indices& edges) const;
// #endif
//
// 	/** Returns a vertex by the local index of the vertex in the cell */
// 	const geom::Point& vertex(Local_index index) const;
//
// #ifdef TRIANGULAR_CELLS_ONLY
// 	template<Local_index index>
// 	const geom::Point& vertex() const
// 	{
// 		static_assert(index < 3, "Index out of bounds");
// 		return vertex(index);
// 	}
// #endif
//
// 	/** CCW iterators */
//
// 	Vertex_iter begin_vertex() const;
// 	Vertex_end_iter end_vertex() const;
//
// 	Edge_iter begin_edge() const;
// 	Edge_end_iter end_edge() const;
//
// 	Face_iter begin_face() const;
// 	Face_end_iter end_face() const;
//
// 	es::util::Iterable<Vertex_iter, Vertex_end_iter> vertices() const;
// 	es::util::Iterable<Edge_iter, Edge_end_iter> edges() const;
// 	es::util::Iterable<Face_iter, Face_end_iter> faces() const;
//
// private:
// 	Index bounding_halfedge() const;
// };
} // namespace es_fe

namespace geom::internal
{
template<>
struct Traits<fe::Element_view<fe::Edge_tag, fe::Mesh2>>
{
	using Index = fe::Local_index;
	using Tag = Segment_tag;
};

template<>
struct Traits<fe::Element_view<fe::Face_tag, fe::Mesh2>>
{
	using Index = fe::Local_index;
#ifdef TRIANGULAR_CELLS_ONLY
	using Tag = Triangle_tag;
#else
	using Tag = Polygon_tag;
#endif
};
} // namespace geom::internal
