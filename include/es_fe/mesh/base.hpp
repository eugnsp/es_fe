#pragma once
#include <es_fe/types.hpp>
#include <es_fe/forward.hpp>
#include <cstddef>
#include <cstdint>
#include <array>
#include <limits>
#include <type_traits>

// Flag indicating that only triangle elements in 2D meshed are used
#define TRIANGULAR_CELLS_ONLY

namespace es_fe
{

template<std::size_t size>
using Index_array = std::array<Index, size>;

// Mesh type selector based on dimension
template<std::size_t dim>
struct Mesh_selector
{
	static_assert(dim == 1 || dim == 2, "Bad mesh dimension");
	using Type = std::conditional_t<dim == 1, Mesh1, Mesh2>;
};

template<std::size_t dim>
using Mesh_t = typename Mesh_selector<dim>::Type;

} // namespace es_fe
