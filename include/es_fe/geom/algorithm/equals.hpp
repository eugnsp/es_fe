#pragma once
#include <es_fe/geom/compare.hpp>
#include <es_fe/types.hpp>

#include <es_util/algorithm.hpp>

namespace es_fe
{
namespace internal
{
template<class Point1, class Point2>
bool equals(const Point1& point1, const Point2& point2, Point_tag, Point_tag)
{
	return is_geom_equal(point1.x(), point2.x()) && is_geom_equal(point1.y(), point2.y());
}

template<class Polygon1, class Polygon2>
bool equals(const Polygon1& polygon1, const Polygon2& polygon2, Polygon_tag, Polygon_tag)
{
	if (polygon1.n_vertices() != polygon2.n_vertices())
		return false;

	const auto equals_pred = [](const auto& point1, const auto& point2) {
		return equals(point1, point2);
	};

	return es_util::is_circ_permutation(
		polygon1.begin_vertex(), polygon1.end_vertex(), polygon2.begin_vertex(), equals_pred);
}
} // namespace internal

template<class Geometry1, class Geometry2>
bool equals(const Geometry1& geometry1, const Geometry2& geometry2)
{
	return internal::equals(
		geometry1, geometry2, typename Geometry1::Geometry_tag{},
		typename Geometry2::Geometry_tag{});
}
} // namespace es_fe
