#pragma once
#include <esf/geometry/point2.hpp>
#include <esf/types.hpp>

#include <esu/error.hpp>

#include <array>
#include <cstddef>
#include <type_traits>
#include <vector>

namespace es_fe::internal
{
// Low-level half-edge datastructure
class Halfedge_structure
{
public:
	using Cell_index = es_fe::Face_index;

	struct Vertex
	{
		explicit Vertex(const es_fe::Point2& pt) : point(pt)
		{}

		Vertex(const es_fe::Point2& pt, es_fe::Halfedge_index halfedge) :
			point(pt), halfedge(halfedge)
		{}

		es_fe::Point2 point;

		// An arbitrary halfedge that starts at the vertex (for a boundary
		// vertex this must always be the outer (boundary) half-edge)
		es_fe::Halfedge_index halfedge = es_fe::Halfedge_index::invalid;
	};

	struct Halfedge
	{
		explicit Halfedge(es_fe::Vertex_index vertex) : vertex(vertex)
		{}

		Halfedge(es_fe::Vertex_index vertex, es_fe::Halfedge_index next_halfedge,
			es_fe::Face_index face) :
			vertex(vertex),
			next(next_halfedge), face(face)
		{}

		// Index of the vertex the half-edge points to
		es_fe::Vertex_index vertex;

		// Index of the next half-edge
		es_fe::Halfedge_index next = es_fe::Halfedge_index::invalid;

		// Index of the face the half-edge belongs to
		es_fe::Face_index face = es_fe::Face_index::invalid;
	};

	struct Face
	{
		explicit Face(es_fe::Halfedge_index halfedge) : halfedge(halfedge)
		{}

		// One of the half-edges bounding the face
		es_fe::Halfedge_index halfedge;
	};

public:
	//////////////////////////////////////////////////////////////////////////
	/** Capacity */

	es_fe::Vertex_index n_vertices() const;
	es_fe::Halfedge_index n_halfedges() const;
	es_fe::Edge_index n_edges() const;
	es_fe::Face_index n_faces() const;
	Cell_index n_cells() const;

	es_fe::Vertex_index n_elements(Vertex_tag) const;
	es_fe::Halfedge_index n_elements(Halfedge_tag) const;
	es_fe::Edge_index n_elements(Edge_tag) const;
	es_fe::Face_index n_elements(Face_tag) const;
	Cell_index n_elements(Cell_tag) const;

	bool is_empty() const;

	void reserve(Index n_vertices, Index n_edges = 0, Index n_faces = 0);
	void shrink();

	// Returns approximate total size of memory in bytes occupied by the data structure
	std::size_t memory_size() const;

	//////////////////////////////////////////////////////////////////////////
	/** Element access */

	const es_fe::Point2& vertex(es_fe::Vertex_index vertex) const
	{
		return vertices_[*vertex].point;
	}

	//////////////////////////////////////////////////////////////////////////
	/** Low level iteration */

	es_fe::Halfedge_index prev(es_fe::Halfedge_index, Vertex_in_circ_tag) const;
	es_fe::Halfedge_index prev(es_fe::Halfedge_index, Vertex_out_circ_tag) const;
	es_fe::Halfedge_index prev(es_fe::Halfedge_index, Face_circ_tag) const;

	es_fe::Halfedge_index next(es_fe::Halfedge_index, Vertex_in_circ_tag) const;
	es_fe::Halfedge_index next(es_fe::Halfedge_index, Vertex_out_circ_tag) const;
	es_fe::Halfedge_index next(es_fe::Halfedge_index, Face_circ_tag) const;

	// Traverses a half-edge loop once and calls the given function
	// 		first			the half-edge to start the loop with
	// 		fn				the callback function, should have the signature
	//						equivalent to void(Halfedge_index)
	//		tag				vertex/face-based circulation tag
	template<typename Fn, class Circ_tag>
	void for_each(es_fe::Halfedge_index first, Fn&& fn, Circ_tag tag) const
	{
		const auto false_pred = [&fn](auto halfedge) {
			fn(halfedge);
			return false;
		};

		find_if(first, false_pred, tag);
	}

	// Traverses a half-edge loop and returns the halfedge index for which the predicate
	// returns true; if the predicate returns true for no halfedge, the invalid index is returned
	// 		first			the half-edge to start the loop with
	// 		pred			the predicate, should have the signature equivalent
	//						to bool(Halfedge_index)
	//		tag				vertex/face-based circulation tag
	template<typename Pred, class Circ_tag>
	es_fe::Halfedge_index find_if(es_fe::Halfedge_index first, Pred&& pred, Circ_tag tag) const
	{
		const auto start = first;

		do
		{
			if (pred(first))
				return first;
			first = next(first, tag);
		} while (first != start);

		return es_fe::Halfedge_index::invalid;
	}

	//////////////////////////////////////////////////////////////////////////

	es_fe::Vertex_index find_vertex(const es_fe::Point2& point) const;
	es_fe::Halfedge_index find_halfedge(es_fe::Vertex_index from, es_fe::Vertex_index to) const;

	es_fe::Halfedge_index halfedge_index(es_fe::Vertex_index vertex) const
	{
		return vertices_[*vertex].halfedge;
	}

	es_fe::Halfedge_index halfedge_index(es_fe::Face_index face) const
	{
		return faces_[*face].halfedge;
	}

	es_fe::Vertex_index vertex_index(es_fe::Halfedge_index halfedge) const
	{
		return halfedges_[*halfedge].vertex;
	}

	es_fe::Face_index face_index(es_fe::Halfedge_index halfedge) const
	{
		return halfedges_[*halfedge].face;
	}

	//////////////////////////////////////////////////////////////////////////
	/** Flags */

	// Checks whether the vertex is a boundary one (the function also returns true if
	// the vertex has an invalid edge index; this can happen only during mesh construction
	// when the vertex is added, but the containing element has not yet been created)
	bool is_boundary(es_fe::Vertex_index) const;

	bool is_boundary(es_fe::Edge_index) const;
	bool is_boundary(es_fe::Halfedge_index) const;
	bool is_boundary(es_fe::Face_index) const;

	//////////////////////////////////////////////////////////////////////////

	void adjust_outgoing_halfedge(es_fe::Vertex_index vertex)
	{
		find_if(
			vertices_[*vertex].halfedge,
			[vertex, this](es_fe::Halfedge_index edge) {
				if (is_boundary(edge))
				{
					vertices_[*vertex].halfedge = edge;
					return true;
				}
				return false;
			},
			Vertex_out_circ_tag{});
	}

	// Adds a vertex and returns the index of the newly added vertex
	es_fe::Vertex_index add_vertex(const es_fe::Point2& point)
	{
		vertices_.emplace_back(point);
		return n_vertices() - 1;
	}

	// Adds a cell and returns the index of the newly added cell (the function
	// automatically creates the missing edges and adjusts the data structure properly),
	es_fe::Face_index add_cell(const std::array<es_fe::Vertex_index, 3>&);

	es_fe::Halfedge_index first_boundary_halfedge() const
	{
		for (es_fe::Halfedge_index i{0}; i < n_halfedges(); ++i)
			if (!is_valid(halfedges_[*i].face))
				return i;

		return es_fe::Halfedge_index::invalid;
	}

	// Clears vertices, edges and cells
	void clear()
	{
		vertices_.clear();
		halfedges_.clear();
		faces_.clear();
	}

	// Performs some basic checks of mesh data structure consistency
	esu::Error check() const;

	// TODO : generic
	void debug_check_index([[maybe_unused]] es_fe::Vertex_index index) const
	{
		assert(index < n_vertices() || !is_valid(index));
	}

	void debug_check_index([[maybe_unused]] es_fe::Edge_index index) const
	{
		assert(index < n_edges() || !is_valid(index));
	}

	void debug_check_index([[maybe_unused]] es_fe::Halfedge_index index) const
	{
		assert(index < n_halfedges() || !is_valid(index));
	}

	void debug_check_index([[maybe_unused]] es_fe::Face_index index) const
	{
		assert(index < n_faces() || !is_valid(index));
	}

private:
	// Adds a pair of half-edges without adjusting the data structure, returns
	// the index of the first half-edge in the newly added pair (the first (second)
	// half-edge points to the vertex2 (vertex1)
	es_fe::Halfedge_index insert_halfedges_raw(
		es_fe::Vertex_index vertex1, es_fe::Vertex_index vertex2)
	{
		halfedges_.emplace_back(vertex1);
		halfedges_.emplace_back(vertex2);
		return n_halfedges() - 2;
	}

	// Adds a cell without adjusting the data structure and
	// returns the index of the newly added cell
	es_fe::Face_index add_face_raw(es_fe::Halfedge_index halfedge)
	{
		faces_.emplace_back(halfedge);
		return n_faces() - 1;
	}

protected:
	std::vector<Vertex> vertices_;
	std::vector<Halfedge> halfedges_;
	std::vector<Face> faces_;
};
} // namespace es_fe::internal
