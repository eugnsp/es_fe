#pragma once
#include <es_fe/types.hpp>
// #include "base.hpp"
// #include "iterator.hpp"
// #include <es_geom/point.hpp>
// #include <es_geom/linear_grid.hpp>
// #include <es_util/iterator.hpp>
// #include <es_util/error.hpp>

#include <cstddef>
#include <utility>
#include <vector>
// #include <string>
// #include <iosfwd>

namespace es_fe
{
class Mesh1
{
public:
	static constexpr std::size_t dim = 1;

public:
	using Cell_index = Edge_index;
// 	using Vertex_view = View<Vertex_tag, Mesh1>;
// 	using Edge_view = View<Edge_tag, Mesh1>;
// 	using Cell_view = Edge_view;
//
// 	using Vertex_iter = Random_access_iterator<Vertex_tag, Mesh1>;
// 	using Edge_iter = Random_access_iterator<Edge_tag, Mesh1>;
// 	using Cell_iter = Edge_iter;
//
public:
	Mesh1() = default;

	Mesh1(const std::vector<double> vertices) : vertices_(std::move(vertices))
	{}

	//////////////////////////////////////////////////////////////////////
	//* Capacity */

	Vertex_index n_vertices() const;
	Edge_index n_edges() const;
 	Cell_index n_cells() const;

 	Vertex_index n_elements(Vertex_tag) const;
	Edge_index n_elements(Edge_tag) const;
	Cell_index n_elements(Cell_tag) const;

// 	void reserve(Index n_vertices);
//
// 	// Returns approximate total size (in bytes) of memory occupied by the mesh
// 	std::size_t memory_size() const;
//
// 	const geom::Point& vertex(Index) const;
// 	void rescale(double);
//
// 	bool is_empty() const;
// 	bool is_bnd_vertex(Index) const;
// 	bool is_bnd_cell(Index) const;
//
// 	// Performs some basic checks of mesh data structure consistency
// 	es::util::Error is_ok() const;
//
// 	//////////////////////////////////////////////////////////////////////////
// 	/** Views */
//
// 	Vertex_view vertex_view(Index) const;
// 	Edge_view edge_view(Index) const;
//
// 	//////////////////////////////////////////////////////////////////////////
// 	/** Iterators */
//
// 	Vertex_iter begin_vertex() const;
// 	Vertex_iter end_vertex() const;
//
// 	es::util::Iterable<Vertex_iter> vertices() const;
//
// 	Edge_iter begin_edge() const;
// 	Edge_iter end_edge() const;
//
// 	es::util::Iterable<Edge_iter> edges() const;
// 	es::util::Iterable<Cell_iter> cells() const;
//
// 	//////////////////////////////////////////////////////////////////////////
//
// 	std::string type_string() const;
//
// private:
// 	void clear();
//
// 	// Adds a new vertex and returns its index
// 	//Index add_vertex(double);
//
private:
	std::vector<double> vertices_;
};
//
// std::ostream& operator<<(std::ostream&, const Mesh1&);
}
