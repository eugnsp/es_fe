#pragma once
#include <es_fe/geometry/point1.hpp>
#include <es_fe/math/gauss/data.hpp>
#include <es_fe/types.hpp>

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

	// Returns the quadrature point with the given index
	// on the reference interval [0, 1]
	static constexpr es_fe::Point1 point(Local_index point)
	{
		assert(point < size());

		// Map the interval [-1, 1], which is used in data, to [0, 1]
		return es_fe::Point1{(1 + points[point].x) / 2};
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
