#pragma once
#include <es_la/dense.hpp>
#include <es_la/function.hpp>

namespace es_fe
{
// Returns the Jacobian of the transformation
// from a given triangle to the reference one
template<typename Triangle>
la::Matrix_2d jacobian(const Triangle& triangle)
{
	auto v = triangle.vertex_circ();
	auto&& a = v->vertex();
	auto&& b = (++v)->vertex();
	auto&& c = (++v)->vertex();

	la::Matrix_2d j;

	j.col<0>() = b - a;
	j.col<1>() = c - a;
	return j;
}

// Returns the inverted Jacobian of the transformation
// from a given triangle to the reference one
template<typename Triangle>
la::Matrix_2d inv_jacobian(const Triangle& triangle)
{
	auto j = jacobian(triangle);
	invert(j);
	return j;
}

// Returns the inverted transposed Jacobian of the transformation
// from a given triangle to the reference one
template<typename Triangle>
la::Matrix_2d inv_transp_jacobian(const Triangle& triangle)
{
	auto j = jacobian(triangle);
	invert_transpose(j);
	return j;
}
} // namespace es_fe
