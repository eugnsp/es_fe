#include <es/fe/mesh/view/view.hpp>
#include <es/fe/mesh.hpp>
#include <es/fe/mesh/iterator.hpp>
#include <es_util/function.hpp>
#include <ostream>
#include <cassert>

namespace fe
{
//////////////////////////////////////////////////////////////////////////
/** 1D mesh views */

// const geom::Point& View<Vertex_tag, Mesh1>::vertex() const
// {
// 	return mesh_.vertex(index_);
// }
// 
// View<Edge_tag, Mesh1> View<Vertex_tag, Mesh1>::_first_cell() const
// {
// 	const auto edge_index = (index_ == 0) ? 0 : index_ - 1;
// 	return View<Edge_tag, Mesh1>{mesh_, edge_index};
// }
// 
// Index View<Edge_tag, Mesh1>::vertex_index(Local_index index) const
// {
// 	assert(index < 2);
// 	return index_ + index;
// }
// 
// void View<Edge_tag, Mesh1>::vertex_indices(Index_array<2>& indices) const
// {
// 	indices[0] = vertex_index(0);
// 	indices[1] = vertex_index(1);
// }
// 
// const geom::Point& View<Edge_tag, Mesh1>::vertex(Local_index index) const
// {
// 	return mesh_.vertex(vertex_index(index));
// }

//////////////////////////////////////////////////////////////////////////
/** 2D mesh vertex view */

const geom::Point& View<Vertex_tag, Mesh2>::vertex() const
{
	return mesh_.vertex(index_);
}

auto View<Vertex_tag, Mesh2>::begin_vertex() const -> Vertex_iter
{
	return Vertex_iter{mesh_, mesh_.outgoing_halfedge(index_)};
}

auto View<Vertex_tag, Mesh2>::end_vertex() const -> Vertex_end_iter
{
	return {};
}

// auto View<Vertex_tag, Mesh2>::begin_edge() const -> Edge_iter
// {
// 	return Edge_iter{mesh_, mesh_.halfedge_index_by_vertex(index_)};
// }

auto View<Vertex_tag, Mesh2>::end_edge() const -> Edge_end_iter
{
	return {};
}
auto View<Vertex_tag, Mesh2>::begin_face() const -> Face_iter
{
	return Face_iter{mesh_, mesh_.outgoing_halfedge(index_)};
}

auto View<Vertex_tag, Mesh2>::end_face() const -> Face_end_iter
{
	return {};
}

auto View<Vertex_tag, Mesh2>::vertices() const -> es::util::Iterable<Vertex_iter, Vertex_end_iter>
{ 
	return {begin_vertex(), end_vertex()};
}

// auto View<Vertex_tag, Mesh2>::edges() const -> es::util::Iterable<Edge_iter, Edge_end_iter>
// {
// 	return {begin_edge(), end_edge()};
// }

auto View<Vertex_tag, Mesh2>::faces() const -> es::util::Iterable<Face_iter, Face_end_iter>
{
	return {begin_face(), end_face()};
}

Index View<Vertex_tag, Mesh2>::n_faces() const
{
	return mesh_.numberOfAdjacentCells(index_);
}

//////////////////////////////////////////////////////////////////////////
/** 2D mesh edge view */

// Index View<Edge_tag, Mesh2>::operator*() const
// {
// 	return index_ / 2;
// }
// 
// Index View<Edge_tag, Mesh2>::half_edge_index() const
// {
// 	return index_;
// }
// 
// Index View<Edge_tag, Mesh2>::vertex_index(Local_index index) const
// {
// 	assert(index < 2);
// 	const auto halfedge = (index == 1) ? index_ : mesh_.twin_halfedge_index(index_);
// 	return mesh_.vertex_index_by_halfedge(halfedge);
// }
// 
// void View<Edge_tag, Mesh2>::vertex_indices(Index_array<2>& indices) const
// {
// 	indices[0] = vertex_index(0);
// 	indices[1] = vertex_index(1);
// }
// 
// Index View<Edge_tag, Mesh2>::vertex_from_index() const
// {
// 	return vertex_index(0);
// }
 
//  Vertex_index View<Halfedge_tag, Mesh2>::vertex_to_index() const
//  {
// 	 // TODO
// 	 throw;
//  	//return vertex_index(1);
//  }
// 
// 
//  Vertex_index View<Edge_tag, Mesh2>::vertex_to_index() const
//  {
// 	 // TODO
// 	 throw;
// 	 //return vertex_index(1);
//  }


// const geom::Point& View<Edge_tag, Mesh2>::vertex(Local_index index) const
// {
// 	return mesh_.vertex(vertex_index(index));
// }
// 
// auto View<Edge_tag, Mesh2>::vertex_view(Local_index index) const -> Vertex_view
// {
// 	return mesh_.vertex_view(vertex_index(index));
// }
// 
// auto View<Edge_tag, Mesh2>::vertex_from_view() const -> Vertex_view
// {
// 	return vertex_view(0);
// }
// 
// auto View<Edge_tag, Mesh2>::vertex_to_view() const -> Vertex_view
// {
// 	return vertex_view(1);
// }
// 
// Index View<Edge_tag, Mesh2>::face_index(Local_index index) const
// {
// 	assert(index < 2);
// 	const auto halfedge = (index == 0) ? index_ : mesh_.twin_halfedge_index(index_);
// 	return mesh_.face_index_by_halfedge(halfedge);
// }
// 
// Index View<Edge_tag, Mesh2>::left_face_index() const
// {
// 	return face_index(0);
// }
// 
// Index View<Edge_tag, Mesh2>::right_face_index() const
// {
// 	return face_index(1);
// }
// 
// auto View<Edge_tag, Mesh2>::face_view(Local_index index) const -> Face_view
// {	
// 	return mesh_.face_view(face_index(index));
// }
// 
// auto View<Edge_tag, Mesh2>::left_face_view() const -> Face_view
// {
// 	return face_view(0);
// }
// 
// auto View<Edge_tag, Mesh2>::right_face_view() const -> Face_view
// {
// 	return face_view(1);
// }

//////////////////////////////////////////////////////////////////////////
/* Face view */

#ifndef TRIANGULAR_CELLS_ONLY
Local_index View<Face_tag, Mesh2>::n_vertices() const
{
	Local_index number = 0;
	mesh_.halfedge_loop(bounding_halfedge(), [&number](Index) { ++number; return false; });
	return number;
}
#endif

Vertex_index View<Face_tag, Mesh2>::vertex_index(Local_index index) const
{
#ifdef TRIANGULAR_CELLS_ONLY			
	assert(index < 3);
#endif

	auto halfedge = bounding_halfedge();
	switch (index)
	{
	case 2:
		halfedge = mesh_.next(halfedge, Face_circ_tag{});
		[[fallthrough]];

	case 1:
		halfedge = mesh_.next(halfedge, Face_circ_tag{});

	#ifndef TRIANGULAR_CELLS_ONLY
		[[fallthrough]];

	case 0:
		break;

	default:
		for (Local_index i = 0; i < index; ++i)
			halfedge = mesh_.next(halfedge, Face_circ_tag{});
	#endif
	}

	return mesh_.halfedges_[*halfedge].vertex;
}

void View<Face_tag, Mesh2>::vertex_indices(std::vector<Vertex_index>& indices) const
{
	assert(indices.empty());

	mesh_.for_each(bounding_halfedge(), 
		[&indices, this](Halfedge_index halfedge)
		{
			indices.push_back(mesh_.vertex_index_by_halfedge(halfedge));
		}, Face_circ_tag{});
}

void View<Face_tag, Mesh2>::vertex_and_edge_indices(std::vector<Vertex_index>& vertices,
													std::vector<Edge_index>& edges) const
{
	assert(vertices.empty());
	assert(edges.empty());

	mesh_.for_each(bounding_halfedge(),
		[&vertices, &edges, this](Halfedge_index halfedge)
		{
			vertices.push_back(mesh_.vertex_index_by_halfedge(halfedge));
			edges.push_back(edge(halfedge));
			return false;
		}, Face_circ_tag{});
}

void View<Face_tag, Mesh2>::vertex_and_edge_indices(std::vector<Vertex_index>& vertices,
													std::vector<std::pair<Edge_index, bool>>& edges) const
{
	assert(vertices.empty());
	assert(edges.empty());

	mesh_.for_each(bounding_halfedge(),
		[&vertices, &edges, this](Halfedge_index halfedge)
		{
			vertices.push_back(mesh_.vertex_index_by_halfedge(halfedge));
			edges.emplace_back(edge(halfedge), mesh_.is_first_halfedge(halfedge));
			return false;
		}, Face_circ_tag{});
}

#ifdef TRIANGULAR_CELLS_ONLY
void View<Face_tag, Mesh2>::vertex_indices(Vertex_indices& vertices) const
{
	auto halfedge = bounding_halfedge();
	vertices[0] = mesh_.vertex_index_by_halfedge(halfedge);

	halfedge = mesh_.next(halfedge, Face_circ_tag{});
	vertices[1] = mesh_.vertex_index_by_halfedge(halfedge);

	halfedge = mesh_.next(halfedge, Face_circ_tag{});
	vertices[2] = mesh_.vertex_index_by_halfedge(halfedge);
}

void View<Face_tag, Mesh2>::edge_indices(Edge_indices& edges) const
{
	auto halfedge = bounding_halfedge();
	edges[0] = edge(halfedge);

	halfedge = mesh_.next(halfedge, Face_circ_tag{});
	edges[1] = edge(halfedge);

	halfedge = mesh_.next(halfedge, Face_circ_tag{});
	edges[2] = edge(halfedge);
}

void View<Face_tag, Mesh2>::vertex_and_edge_indices(
	Vertex_indices& vertices, Edge_indices& edges) const
{
	auto halfedge = bounding_halfedge();
	vertices[0] = mesh_.vertex_index_by_halfedge(halfedge);
	edges[0] = edge(halfedge);

	halfedge = mesh_.next(halfedge, Face_circ_tag{});
	vertices[1] = mesh_.vertex_index_by_halfedge(halfedge);
	edges[1] = edge(halfedge);

	halfedge = mesh_.next(halfedge, Face_circ_tag{});
	vertices[2] = mesh_.vertex_index_by_halfedge(halfedge);
	edges[2] = edge(halfedge);
}

void View<Face_tag, Mesh2>::vertex_and_edge_indices(
	Vertex_indices& vertices, Directed_edge_indices& edges) const
{
	auto halfedge = bounding_halfedge();
	vertices[0] = mesh_.vertex_index_by_halfedge(halfedge);
	edges[0] = {edge(halfedge), mesh_.is_first_halfedge(halfedge)};

	halfedge = mesh_.next(halfedge, Face_circ_tag{});
	vertices[1] = mesh_.vertex_index_by_halfedge(halfedge);
	edges[1] = {edge(halfedge), mesh_.is_first_halfedge(halfedge)};

	halfedge = mesh_.next(halfedge, Face_circ_tag{});
	vertices[2] = mesh_.vertex_index_by_halfedge(halfedge);
	edges[2] = {edge(halfedge), mesh_.is_first_halfedge(halfedge)};
}
#endif

const geom::Point& View<Face_tag, Mesh2>::vertex(Local_index index) const
{
	return mesh_.vertex(vertex_index(index));
}

auto View<Face_tag, Mesh2>::begin_vertex() const -> Vertex_iter
{
	return Vertex_iter{mesh_, bounding_halfedge()};
}

auto View<Face_tag, Mesh2>::end_vertex() const -> Vertex_end_iter
{
	return {};
}

// auto View<Face_tag, Mesh2>::begin_edge() const -> Edge_iter
// {
// 	return Edge_iter{mesh_, bounding_halfedge()};
// }

auto View<Face_tag, Mesh2>::end_edge() const -> Edge_end_iter
{
	return {};
}

auto View<Face_tag, Mesh2>::begin_face() const -> Face_iter
{
	return Face_iter{mesh_, bounding_halfedge()};
}

auto View<Face_tag, Mesh2>::end_face() const -> Face_end_iter
{
	return {};
}

auto View<Face_tag, Mesh2>::vertices() const -> es::util::Iterable<Vertex_iter, Vertex_end_iter>
{
	return {begin_vertex(), end_vertex()};
}

// auto View<Face_tag, Mesh2>::edges() const -> es::util::Iterable<Edge_iter, Edge_end_iter>
// {
// 	return {begin_edge(), end_edge()};
// }

auto View<Face_tag, Mesh2>::faces() const -> es::util::Iterable<Face_iter, Face_end_iter>
{
	return {begin_face(), end_face()};
}

Halfedge_index View<Face_tag, Mesh2>::bounding_halfedge() const
{
	return mesh_.faces_[*index_].halfedge;
}

//////////////////////////////////////////////////////////////////////////
/** View display on the screen */

// std::ostream& operator<<(std::ostream& os, const View<Vertex_tag, Mesh1>& vertex)
// {
// 	os << "Vertex #" << *vertex << ": (" << vertex.vertex() << ')';
// 	return os;
// }

std::ostream& operator<<(std::ostream& os, const View<Vertex_tag, Mesh2>& vertex)
{
	os << "Vertex #" << **vertex << ": " << vertex.vertex();
	return os;
}

std::ostream& operator<<(std::ostream& os, const View<Edge_tag, Mesh2>& edge)
{
	// TODO
	throw;
// 	os << "Edge #" << **edge << ": "
// 		<< edge.vertex_from_index() << ' ' << edge.vertex_from() << " -> "
// 		<< edge.vertex_to_index() << ' ' << edge.vertex_to();

	return os;
}

std::ostream& operator<<(std::ostream& os, const View<Face_tag, Mesh2>& cell)
{
	// TODO
	throw;

	std::vector<Vertex_index> vertices;
	cell.vertex_indices(vertices);

	//os << "Cell #" << **cell << ": [" << es::util::join_as_string(vertices.begin(), vertices.end()) << "]\n";
// 	for (auto edge = cell.begin_edge(); edge != cell.end_edge(); ++edge)
// 		os << *edge << '\n';

	return os;
}
}


// 	Index View<Edge_tag, Mesh2>::oppositeVertexIndex(Local_index index) const
// 	{
// 		assert(index < 2);
// 
// 		const auto edge = (index == 0) ? index_ : mesh_.twin_halfedge_index(index_);
// 
// 		return mesh_.is_bnd_halfedge(edge) ?
// 			invalid_index : mesh_.halfedges_[mesh_.next_halfedge_index(edge)].vertex;
// 	}
