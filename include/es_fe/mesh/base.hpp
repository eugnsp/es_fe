#pragma once
#include <es_fe/types.hpp>
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
} // namespace es_fe
