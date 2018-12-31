#include <es_fe/types.hpp>
#include <es_fe/mesh/mesh2.hpp>
#include <es_fe/geom/point.hpp>

namespace es_fe
{
const Point& Element_view<Vertex_tag, Mesh2>::vertex() const
{
	return mesh_.vertex(index_);
}

auto Element_view<Vertex_tag, Mesh2>::vertex_circ() const -> Vertex_circ
{
	return Vertex_circ{mesh_, mesh_.halfedge_index(index_)};
}

auto Element_view<Vertex_tag, Mesh2>::halfedge_in_circ() const -> Halfedge_in_circ
{
	return Halfedge_in_circ{mesh_, twin(mesh_.halfedge_index(index_))};
}

auto Element_view<Vertex_tag, Mesh2>::halfedge_out_circ() const -> Halfedge_out_circ
{
	return Halfedge_out_circ{mesh_, mesh_.halfedge_index(index_)};
}

auto Element_view<Vertex_tag, Mesh2>::edge_circ() const -> Edge_circ
{
	return Edge_circ{mesh_, mesh_.halfedge_index(index_)};
}

auto Element_view<Vertex_tag, Mesh2>::face_circ() const -> Face_circ
{
	return Face_circ{mesh_, mesh_.halfedge_index(index_)};
}

Local_index Element_view<Vertex_tag, Mesh2>::n_edges() const
{
	auto he = halfedge_in_circ();
	const auto he_first = he;

	Local_index n = 0;
	do
		++n;
	while (++he != he_first);

	return n;
}

Local_index Element_view<Vertex_tag, Mesh2>::n_faces() const
{
	return n_edges() - is_boundary();
}

// Returns the number of neighbouring faces
// Index n_faces() const;

} // namespace es_fe