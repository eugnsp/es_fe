#pragma once
#include <es_fe/forward.hpp>
#include <es_fe/types.hpp>
#include <es_fe/math/gauss/data.hpp>
#include <cassert>

namespace es_fe
{
template<Local_index order>
class Quadr<order, 1>
{
public:
	static constexpr Local_index dim = 1;

public:
	// Returns the total number of quadrature points
	static constexpr Local_index size()
	{
		return static_cast<Local_index>(points.size());
	}

	// Returns the quadrature point that has index (point) on
	// edge (0, 0) - (1, 0) of the reference triangle
	static constexpr la::Vector_2d point(Local_index point)
	{
		assert(point < size());

		// Map from [-1, 1] to [0, 1]
		return la::Vector_2d({(1 + points[point].x) / 2, 0});
	}

	// Computes a weighted sum over quadrature points,
	// sum_i weight(i) * fn(i) for i = 0, ... size() - 1
	template<class Fn>
	static constexpr auto sum(Fn fn)
	{
		return sum_impl(fn, Make_local_index_sequence<size()>{});
	}

private:
	static constexpr auto points = internal::Gauss_data<order>::points;

private:
	template<class Fn, Local_index... indices>
	static constexpr auto sum_impl(Fn fn, Local_index_sequence<indices...>)
	{
		return (... + (points[indices].weight * fn(indices)));
	}
};
} // namespace es_fe
