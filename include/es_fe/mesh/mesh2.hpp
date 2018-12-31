#pragma once
#include <es_fe/types.hpp>
#include <es_fe/type_traits.hpp>
#include <es_fe/mesh/halfedge_structure.hpp>
#include <es_fe/mesh/view/vertex_mesh2.hpp>
#include <es_fe/mesh/view/halfedge_mesh2.hpp>
#include <es_fe/mesh/view/edge_mesh2.hpp>
#include <es_fe/mesh/view/face_mesh2.hpp>
#include <es_fe/mesh/iterator/circulator_halfedge_edge.hpp>
#include <es_fe/mesh/iterator/circulator_vertex_face.hpp>
#include <es_fe/mesh/iterator/random_access.hpp>
#include <es_fe/geom/point.hpp>
#include <es_fe/geom/rect.hpp>

#include <es_util/error.hpp>
#include <es_util/iterator.hpp>

#include <cassert>
#include <cstddef>
#include <iosfwd>
#include <string>
#include <type_traits>
#include <utility>

// enum class MeshObserveCode
// {
// 	BEFORE_REFINE,
// 	REFINE_CELL,
// 	AFTER_REFINE
// };

namespace es_fe
{
class Mesh2 : public internal::Halfedge_structure
{
	friend class Element_view<Vertex_tag, Mesh2>;
	friend class Element_view<Edge_tag, Mesh2>;
	friend class Element_view<Face_tag, Mesh2>;

public:
	static constexpr std::size_t dim = 2;

public:
	using Vertex_view = Element_view<Vertex_tag, Mesh2>;
	using Halfedge_view = Element_view<Halfedge_tag, Mesh2>;
	using Edge_view = Element_view<Edge_tag, Mesh2>;
	using Face_view = Element_view<Face_tag, Mesh2>;
	using Cell_view = Face_view;

	using Vertex_iter = Random_access_iterator<Vertex_tag, Mesh2>;
	using Halfedge_iter = Random_access_iterator<Halfedge_tag, Mesh2>;
	using Edge_iter = Random_access_iterator<Edge_tag, Mesh2>;
	using Face_iter = Random_access_iterator<Face_tag, Mesh2>;
	using Cell_iter = Face_iter;

	using Boundary_vertex_circ = Circulator<Vertex_tag, Face_circ_tag>;
	using Boundary_halfedge_circ = Circulator<Halfedge_tag, Face_circ_tag>;
	using Boundary_edge_circ = Circulator<Edge_tag, Face_circ_tag>;

private:
	using Base = internal::Halfedge_structure;

public:
	virtual ~Mesh2() = default;

	//////////////////////////////////////////////////////////////////////////

	Vertex_index vertex_index_by_halfedge(Halfedge_index) const;

	static Index first_halfedge_index_by_edge(Index);
	static Index second_halfedge_index_by_edge(Index);

	static bool is_first_halfedge(Halfedge_index);
	static bool is_second_halfedge(Halfedge_index);

	Halfedge_index halfedge_index_by_vertex(Vertex_index) const;

	Face_index face_index(Halfedge_index index) const
	{
		return halfedges_[*index].face;
	}

	Halfedge_index halfedge_from_face(Face_index face) const
	{
		return faces_[*face].halfedge;
	}

	virtual Vertex_index find_vertex(const Point& point) const
	{
		return Base::find_vertex(point);
	}

	/** Returns index of the cell the half-edge belongs to */
	// 				IndexType cell(IndexType edge) const
	// 				{
	// 					return edges_[edge].cell_;
	// 				}

	// Returns global vertex indices on the edge
	std::pair<Index, Index> vertexIndicesOnEdge(Index edge) const
	{
		return {*halfedges_[2 * edge].vertex, *halfedges_[2 * edge + 1].vertex};
	}

	//////////////////////////////////////////////////////////////////////////
	/** Flags */

	//////////////////////////////////////////////////////////////////////////
	/** Views */

	Vertex_view view(Vertex_index) const;
	Halfedge_view view(Halfedge_index) const;
	Edge_view view(Edge_index) const;
	Face_view view(Face_index) const;

	//////////////////////////////////////////////////////////////////////////
	/** Iterators */

	Vertex_iter begin_vertex() const;
	Vertex_iter end_vertex() const;
	es_util::Iterable<Vertex_iter> vertices() const;

	Halfedge_iter begin_halfedge() const;
	Halfedge_iter end_halfedge() const;
	es_util::Iterable<Halfedge_iter> halfedges() const;

	Edge_iter begin_edge() const;
	Edge_iter end_edge() const;
	es_util::Iterable<Edge_iter> edges() const;

	Face_iter begin_face() const;
	Face_iter end_face() const;
	es_util::Iterable<Face_iter> faces() const;

	Cell_iter begin_cell() const;
	Cell_iter end_cell() const;
	es_util::Iterable<Cell_iter> cells() const;

	Boundary_vertex_circ boundary_vertex_circ() const;
	Boundary_halfedge_circ boundary_halfedge_circ() const;
	Boundary_edge_circ boundary_edge_circ() const;

	//////////////////////////////////////////////////////////////////////////

	// Returns the smallest rectangle that contains the mesh
	Rect bounding_rect() const;

	//////////////////////////////////////////////////////////////////////////

	Index numberOfAdjacentCells(Vertex_index vertex) const
	{
		Index number = 0;
		for_each(vertices_[*vertex].halfedge, [&number](auto) { ++number; }, Vertex_out_circ_tag{});

		number -= is_boundary(vertex);

		assert(number > 0);
		return number;
	}

	virtual std::string type_string() const
	{
		return "2D unstructured mesh";
	};

	// Performs some basic checks of mesh data structure consistency
	es_util::Error check() const;

	// Clears edges and cells preserving vertices
	void clearEdgesAndCells()
	{
		halfedges_.clear();
		faces_.clear();
	}

	void scale(double scale)
	{
		for (auto& v : vertices_)
			v.point *= scale;
	}

	// Outputs human readable information about the mesh
	void print(std::ostream&) const;

private:
	Halfedge_index outgoing_halfedge(Vertex_index vertex) const
	{
		return vertices_[*vertex].halfedge;
	}

	Halfedge_index ingoing_halfedge(Vertex_index vertex) const
	{
		return twin(outgoing_halfedge(vertex));
	}
};

// Outputs human readable information about the mesh
inline std::ostream& operator<<(std::ostream& os, const Mesh2& mesh)
{
	mesh.print(os);
	return os;
}

//////////////////////////////////////////////////////////////////////////

inline Vertex_index n_elements(const Mesh2& mesh, Vertex_tag)
{
	return mesh.n_vertices();
}

inline Edge_index n_elements(const Mesh2& mesh, Edge_tag)
{
	return mesh.n_edges();
}

inline Face_index n_elements(const Mesh2& mesh, Face_tag)
{
	return mesh.n_faces();
}
} // namespace es_fe