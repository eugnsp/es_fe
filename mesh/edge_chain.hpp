#pragma once
#include "e_geom/forward.hpp"
#include "mesh2.hpp"	// TODO ? : remove
#include "edge_chain_iterator.hpp"
#include <vector>

namespace fe
{
class Edge_chain
{
public:
	// TODO : remove
	Edge_chain(const Mesh2& mesh) : mesh_(mesh) { }

	Edge_chain(const Mesh2& mesh, const geom::Boundary&);

	// 			EdgeChainVertexIterator begin_vertex() const
	// 			{ }
	// 
	// 			EdgeChainVertexIterator end_vertex() const
	// 			{ }

	// TODO : remove
	void push_back(const Mesh2::Edge_view& edge)
 	{
 		half_edges_.push_back(edge.half_edge_index());
 	}

	Edge_chain_edge_iterator begin() const
	{
		return {mesh_, half_edges_.begin(), half_edges_.end()};
	}

	Edge_chain_edge_iterator end() const
	{
		return {mesh_, half_edges_.end(), half_edges_.end()};
 	}
 
private:
	std::vector<Index> half_edges_;
	const Mesh2& mesh_;
};
}
