#pragma once
#include <cassert>
#include <cstdint>
#include <utility>

namespace es_fe
{
//////////////////////////////////////////////////////////////////////////
/** Mesh element tags */

struct Vertex_tag
{};
struct Halfedge_tag
{};
struct Edge_tag
{};
struct Face_tag
{};
struct Cell_tag		// Edge in 1D, Face in 2D
{};

//////////////////////////////////////////////////////////////////////////
/** General indices */

using Index = std::uint32_t;
using Local_index = unsigned int;

inline constexpr auto invalid_index = static_cast<Index>(-1);
inline constexpr auto invalid_local_index = static_cast<Local_index>(-1);

template<Local_index... indices>
using Local_index_sequence = std::integer_sequence<Local_index, indices...>;

template<Local_index n>
using Make_local_index_sequence = std::make_integer_sequence<Local_index, n>;

//////////////////////////////////////////////////////////////////////////
/** Mesh element indices */

#define ES_FE_DEFINE_INDEX_TYPE(Index_name)                                                        \
	enum class Index_name : Index                                                                  \
	{                                                                                              \
		invalid = invalid_index                                                                    \
	};                                                                                             \
                                                                                                   \
	inline Index operator*(Index_name index)                                                       \
	{                                                                                              \
		assert(index != Index_name::invalid);                                                      \
		return static_cast<Index>(index);                                                          \
	}                                                                                              \
                                                                                                   \
	template<typename N>                                                                           \
	inline Index_name& operator+=(Index_name& index, N n)                                          \
	{                                                                                              \
		index = Index_name{*index + static_cast<Index>(n)};                                        \
		return index;                                                                              \
	}                                                                                              \
                                                                                                   \
	template<typename N>                                                                           \
	inline Index_name& operator-=(Index_name& index, N n)                                          \
	{                                                                                              \
		index = Index_name{*index - static_cast<Index>(n)};                                        \
		return index;                                                                              \
	}                                                                                              \
                                                                                                   \
	template<typename N>                                                                           \
	inline Index_name operator+(Index_name index, N n)                                             \
	{                                                                                              \
		return index += n;                                                                         \
	}                                                                                              \
                                                                                                   \
	template<typename N>                                                                           \
	inline Index_name operator-(Index_name index, N n)                                             \
	{                                                                                              \
		return index -= n;                                                                         \
	}                                                                                              \
                                                                                                   \
	inline Index_name& operator++(Index_name& index)                                               \
	{                                                                                              \
		return index += 1;                                                                         \
	}                                                                                              \
                                                                                                   \
	inline Index_name& operator--(Index_name& index)                                               \
	{                                                                                              \
		return index -= 1;                                                                         \
	}                                                                                              \
                                                                                                   \
	inline bool is_valid(Index_name index)                                                         \
	{                                                                                              \
		return index != Index_name::invalid;                                                       \
	}

ES_FE_DEFINE_INDEX_TYPE(Vertex_index)
ES_FE_DEFINE_INDEX_TYPE(Halfedge_index)
ES_FE_DEFINE_INDEX_TYPE(Edge_index)
ES_FE_DEFINE_INDEX_TYPE(Face_index)

// TODO : move to somewhere else
inline Halfedge_index twin(Halfedge_index index)
{
	return Halfedge_index{*index ^ 1};
}

inline Edge_index edge(Halfedge_index index)
{
	return Edge_index{*index / 2};
}

inline Halfedge_index first_halfedge(Edge_index index)
{
	return Halfedge_index{2 * *index};
}

inline Halfedge_index second_halfedge(Edge_index index)
{
	return first_halfedge(index) + 1;
}

inline bool is_first_halfedge(Halfedge_index index)
{
	return *index % 2 == 0;
}

inline bool is_second_halfedge(Halfedge_index index)
{
	return !is_first_halfedge(index);
}

//////////////////////////////////////////////////////////////////////////
/** Circulation type tags */

struct Vertex_out_circ_tag
{};
struct Vertex_in_circ_tag
{};
struct Face_circ_tag
{};
} // namespace es_fe