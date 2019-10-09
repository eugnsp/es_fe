#pragma once
#include <esf/geometry.hpp>
#include <esf/mesh/mesh2.hpp>
#include <esf/math/jacobian.hpp>

#include <esl/dense.hpp>

namespace es_fe::internal
{
// Maps a given point on given face to a point in the corresponding reference triangle
inline es_fe::Point2 point_to_ref_triangle(const es_fe::Point2& pt, const Mesh2::Face_view& face)
{
	const auto j = es_fe::inv_jacobian(face);
	esl::Vector_2d p0 = pt - face.vertex_circ()->vertex();
	return j * p0;
}
}
