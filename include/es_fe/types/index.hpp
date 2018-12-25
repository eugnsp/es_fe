#pragma once
#include <cassert>
#include <cstdint>
#include <utility>

namespace es_fe
{
using Index = std::uint32_t;
using Local_index = unsigned int;

inline constexpr auto invalid_index = static_cast<Index>(-1);
inline constexpr auto invalid_local_index = static_cast<Local_index>(-1);

template<Local_index... indices>
using Local_index_sequence = std::integer_sequence<Local_index, indices...>;

template<Local_index n>
using Make_local_index_sequence = std::make_integer_sequence<Local_index, n>;

#define ES_FE_DEFINE_INDEX_TYPE(Index_name, Literal_name)                                          \
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
	inline Index_name operator++(Index_name& index, int)                                           \
	{                                                                                              \
		const auto old = index;                                                                    \
		++index;                                                                                   \
		return old;                                                                                \
	}                                                                                              \
                                                                                                   \
	inline Index_name& operator--(Index_name& index)                                               \
	{                                                                                              \
		return index -= 1;                                                                         \
	}                                                                                              \
                                                                                                   \
	inline Index_name operator--(Index_name& index, int)                                           \
	{                                                                                              \
		const auto old = index;                                                                    \
		++index;                                                                                   \
		return old;                                                                                \
	}                                                                                              \
                                                                                                   \
	inline bool is_valid(Index_name index)                                                         \
	{                                                                                              \
		return index != Index_name::invalid;                                                       \
	}                                                                                              \
                                                                                                   \
	namespace literals                                                                             \
	{                                                                                              \
	inline constexpr auto operator"" _##Literal_name(unsigned long long index)                     \
	{                                                                                              \
		return static_cast<Index_name>(index);                                                     \
	}                                                                                              \
	}

ES_FE_DEFINE_INDEX_TYPE(Vertex_index, vi)
ES_FE_DEFINE_INDEX_TYPE(Halfedge_index, hi)
ES_FE_DEFINE_INDEX_TYPE(Edge_index, ei)
ES_FE_DEFINE_INDEX_TYPE(Face_index, fi)

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
} // namespace es_fe
