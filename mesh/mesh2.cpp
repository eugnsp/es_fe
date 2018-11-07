#include <es/fe/mesh.hpp>
#include <es/geom/geometry.hpp>
#include <es_util/function.hpp>
#include <set>
#include <fstream>
#include <string>
#include <algorithm>
#include <limits>
#include <ostream>

namespace fe
{
namespace
{
template<class Index>
std::string indexString(Index index)
{
	return is_valid(index) ? std::to_string(*index) : "<invalid index>";
}
}

//////////////////////////////////////////////////////////////////////////
/** Low level index-based element access */

Vertex_index Mesh2::vertex_index_by_halfedge(Halfedge_index index) const
{
	assert(index < n_halfedges());
	return halfedges_[*index].vertex;
}


bool Mesh2::is_first_halfedge(Halfedge_index index)
{
	return *index % 2 == 0;
}

bool Mesh2::is_second_halfedge(Halfedge_index index)
{
	return !is_first_halfedge(index);
}

Halfedge_index Mesh2::halfedge_index_by_vertex(Vertex_index index) const
{
	assert(index < n_vertices());
	return vertices_[*index].halfedge;
}

Face_index Mesh2::face_index_by_halfedge(Halfedge_index index) const
{
	assert(index < n_halfedges());
	return halfedges_[*index].face;
}

Face_index Mesh2::face_index_by_twin_halfedge(Halfedge_index index) const
{
	return face_index_by_halfedge(twin(index));
}

//////////////////////////////////////////////////////////////////////////
/** Flags */

bool Mesh2::is_empty() const
{
	return vertices_.empty() || halfedges_.empty() || faces_.empty();
}

//////////////////////////////////////////////////////////////////////////



geom::Rect Mesh2::bounding_box() const
{
	double x_min = std::numeric_limits<double>::max();
	double x_max = std::numeric_limits<double>::min();

	double y_min = std::numeric_limits<double>::max();
	double y_max = std::numeric_limits<double>::min();

	// TODO
	//for (auto vertex = begin_bnd_vertex(); vertex != end_bnd_vertex(); ++vertex)
	for (auto vertex = begin_vertex(); vertex != end_vertex(); ++vertex)
	{
		auto& v = vertex->vertex();
		x_min = std::min(x_min, v.x());
		x_max = std::max(x_max, v.x());
		y_min = std::min(y_min, v.y());
		y_max = std::max(y_max, v.y());
	}

	return {{x_min, y_min}, {x_max, y_max}};
}

//////////////////////////////////////////////////////////////////////////
/** Views */

auto Mesh2::view(Vertex_index index) const -> Vertex_view
{
	assert(index < n_vertices() || !is_valid(index));
	return Vertex_view{*this, index};
}

auto Mesh2::view(Edge_index index) const -> Edge_view
{
	assert(index < n_edges() || !is_valid(index));
	return Edge_view{*this, index};
}

auto Mesh2::view(Face_index index) const -> Face_view
{
	assert(index < n_faces() || !is_valid(index));
	return Face_view{*this, index};
}

//////////////////////////////////////////////////////////////////////////
/** Iterators */


auto Mesh2::begin_bnd_vertex() const -> BoundaryVertexIter
{
	return BoundaryVertexIter{*this, first_boundary_halfedge()};
}

auto Mesh2::end_bnd_vertex() const -> BoundaryVertexIter
{
	return BoundaryVertexIter{*this};
}

// auto Mesh2::begin_bnd_edge() const -> Bnd_edge_iter
// {
// 	return Bnd_edge_iter{*this, *first_boundary_halfedge()};
// }

auto Mesh2::end_bnd_edge() const -> Bnd_edge_end_iter
{
	return {};
}


auto Mesh2::bnd_vertices() const -> es::util::Iterable<BoundaryVertexIter>
{
	return {begin_bnd_vertex(), end_bnd_vertex()};
}

// auto Mesh2::bnd_edges() const -> es::util::Iterable<Bnd_edge_iter, Bnd_edge_end_iter>
// {
// 	return {begin_bnd_edge(), end_bnd_edge()};
// }

//////////////////////////////////////////////////////////////////////////
es::util::Error Mesh2::check() const
{
	es::util::Error err("Mesh check");

	if (*n_vertices() == 0)
		err << es::util::error_flag << "\nThe mesh contains no vertices";

	if (*n_edges() == 0)
		err << es::util::error_flag << "\nThe mesh contains no edges";

	if (*n_cells() == 0)
		err << es::util::error_flag << "\nThe mesh contains no faces";

	if (!err.is_OK())
		return err;

	// Check vertices
	for (Index i = 0; i < *n_vertices(); ++i)
		if (vertices_[i].halfedge >= n_halfedges())
			err << es::util::error_flag << '\n' << "The vertex #" << i << " has bad half-edge index (" << indexString(vertices_[i].halfedge) << ')';

	if (!err.is_OK())
		return err;

	// Check edges
	for (Index i = 0; i < *n_halfedges(); ++i)
	{
		auto& edge = halfedges_[i];

		if (edge.vertex >= n_vertices())
			err << es::util::error_flag << '\n' << "The half-edge #" << i << " has bad vertex index (" << indexString(edge.vertex) << ')';

		if (edge.next >= n_halfedges())
			err << es::util::error_flag << '\n' << "The half-edge #" << i << " has bad next half-edge index (" << indexString(edge.next) << ')';

		if (is_valid(edge.face) && edge.face >= n_cells())
			err << es::util::error_flag << '\n' << "The half-edge #" << i << " has bad cell index (" << indexString(edge.face) << ')';
	}

	if (!err.is_OK())
		return err;

	// Check faces
	for (Index i = 0; i < *n_cells(); ++i)
		if (faces_[i].halfedge >= n_halfedges())
			err << es::util::error_flag << '\n' << "The face #" << i << " has bad half-edge index (" << indexString(faces_[i].halfedge) << ')';

	if (!err.is_OK())
		return err;

	// Check face-based cycles
	for (Index i = 0; i < *n_cells(); ++i)
	{
		std::set<Index> seenIndices;

		find(faces_[i].halfedge, [i, &seenIndices, &err](Halfedge_index edge)
		{
			if (seenIndices.count(*edge))
			{
				err << es::util::error_flag << '\n' << "The half-edges of the face #" << i << " do not form a cycle";
				return true;
			}

			seenIndices.insert(*edge);
			return false;
			}, Face_circ_tag{});
	}

	if (!err.is_OK())
		return err;

	// Check vertex-based cycles
	for (Index i = 0; i < *n_vertices(); ++i)
	{
		std::set<Halfedge_index> seenIndices;

		find(vertices_[i].halfedge, 
			[i, &seenIndices, &err](Halfedge_index edge)
			{
				if (seenIndices.count(edge))
				{
					err << es::util::error_flag << '\n' << "The half-edges of the vertex #" << i << " do not form a cycle";
					return true;
				}

				seenIndices.insert(edge);
				return false;
			}, Vertex_out_circ_tag{});
	}

	if (!err.is_OK())
		return err;

	// Check for duplicated vertices
	using Vertices = std::vector<std::pair<Index, geom::Point>>;
	Vertices vertices;
	vertices.reserve(*n_vertices());

	for (Index i = 0; i < *n_vertices(); ++i)
		vertices.emplace_back(i, vertices_[i].point);

	std::sort(vertices.begin(), vertices.end(),
			  [](auto& v1, auto& v2) { return v1.second < v2.second; });

	auto pos = vertices.begin();
	while (pos != vertices.end())
	{
		pos = std::adjacent_find(pos, vertices.end(),
								 [](auto& v1, auto& v2) { return v1.second == v2.second; });

		if (pos == vertices.end())
			break;

		err << es::util::error_flag << '\n' << "Vertices #" << pos->first << " and #" << (pos + 1)->first
			<< " are identical (" << pos->second << ')';

		++pos;
	}

	// Check for unused nodes
	std::vector<bool> seen_nodes(*n_vertices(), false);
	std::for_each(halfedges_.begin(), halfedges_.end(),
		[&seen_nodes](auto& edge) { seen_nodes[*edge.vertex] = true; });

	for (Index i = 0; i < *n_vertices(); ++i)
		if (!seen_nodes[i])
			err << es::util::error_flag << '\n' << "Vertex #" << i << " is unused (" << vertices_[i].point << ')';

	// Check for counter clockwise order of vertices
	for (auto& face : this->faces())
		if (geom::area(face) < 0)
			err << es::util::error_flag << '\n' << "face #" << **face << " has incorrect vertex order";

	return err;
}

// Return human readable information about the mesh
std::ostream& operator<<(std::ostream& ost, const Mesh2& mesh)
{
	ost << mesh.type_string() << '\n'
		<< "Vertices: " << *mesh.n_vertices() << '\n'
		<< "Edges:    " << *mesh.n_edges() << '\n'
		<< "Faces:    " << *mesh.n_faces() << '\n'
		<< "Memory:   " << es::util::size_string(mesh.memory_size()) << std::endl;

	return ost;
}
}