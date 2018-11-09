#pragma once
#include <es_fe/mesh/halfedge_structure/halfedge_structure.hpp>

#include <utility>
#include <type_traits>

namespace es_fe::internal
{
inline auto Halfedge_structure::prev(Halfedge_index index, Vertex_in_circ_tag) const
	-> Halfedge_index
{
	return prev(twin(index), Face_circ_tag{});
}

inline auto Halfedge_structure::prev(Halfedge_index index, Vertex_out_circ_tag) const
	-> Halfedge_index
{
	return twin(prev(index, Face_circ_tag{}));
}

inline auto Halfedge_structure::prev(Halfedge_index index, Face_circ_tag) const -> Halfedge_index
{
	// TO DO : use loop?

	auto prev = index;
	auto curr = index;

	do
	{
		prev = curr;
		curr = next(prev, Face_circ_tag{});
	} while (curr != index);

	return prev;
}

inline auto Halfedge_structure::next(Halfedge_index index, Vertex_in_circ_tag) const
	-> Halfedge_index
{
	return twin(next(index, Face_circ_tag{}));
}

inline auto Halfedge_structure::next(Halfedge_index index, Vertex_out_circ_tag) const
	-> Halfedge_index
{
	return next(twin(index), Face_circ_tag{});
}

inline auto Halfedge_structure::next(Halfedge_index index, Face_circ_tag) const -> Halfedge_index
{
	return halfedges_[*index].next;
}

template<typename Fn, class Circ_tag>
auto Halfedge_structure::find(Halfedge_index first, Fn&& fn, Circ_tag circ_tag) const
	-> Halfedge_index
{
	const auto start = first;

	do
	{
		if (fn(first))
			return first;
		first = next(first, Circ_tag{});
	} while (first != start);

	return Halfedge_index::invalid;
}

template<typename Fn, class Circ_tag>
void Halfedge_structure::for_each(Halfedge_index first, Fn&& fn, Circ_tag circ_tag) const
{
	find(first,
		 [&fn](auto halfedge) {
			 fn(halfedge);
			 return false;
		 },
		 Circ_tag{});
}
} // namespace es_fe::internal