#pragma once
#include <es_fe/mesh/halfedge_structure/halfedge_structure.hpp>

namespace es_fe::internal
{
inline Vertex_index Halfedge_structure::n_vertices() const
{
	return static_cast<Vertex_index>(vertices_.size());
}

inline Halfedge_index Halfedge_structure::n_halfedges() const
{
	return static_cast<Halfedge_index>(halfedges_.size());
}

inline Edge_index Halfedge_structure::n_edges() const
{
	return edge(n_halfedges());
}

inline Face_index Halfedge_structure::n_faces() const
{
	return static_cast<Face_index>(faces_.size());
}

inline auto Halfedge_structure::n_cells() const -> Cell_index
{
	return n_faces();
}

inline void Halfedge_structure::reserve(Index n_vertices, Index n_edges, Index n_faces)
{
	// TODO
	vertices_.reserve(n_vertices);
	// halfedges_.reserve(2 * n_edges);
	faces_.reserve(n_faces);
}

// Returns approximate total size of memory in bytes
// occupied by the data structure
inline std::size_t Halfedge_structure::memory_size() const
{
	// TODO
	return vertices_.capacity() * sizeof(vertices_[0])
		   //+ halfedges_.capacity() * sizeof(halfedges_[0])
		   + faces_.capacity() * sizeof(faces_[0]);
}
} // namespace es_fe::internal