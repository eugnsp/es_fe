#pragma once
#include <es_fe/geometry.hpp>
#include <es_fe/mesh/mesh2.hpp>
#include <es_fe/math/jacobian.hpp>

#include <es_la/dense.hpp>

namespace es_fe::internal
{
// Maps a given point on given face to a point in the corresponding reference triangle
inline es_fe::Point2 point_to_ref_triangle(const es_fe::Point2& pt, const Mesh2::Face_view& face)
{
	const auto j = es_fe::inv_jacobian(face);
	es_la::Vector_2d p0 = pt - face.vertex_circ()->vertex();
	return j * p0;
}
}
