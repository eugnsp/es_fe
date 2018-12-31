#pragma once
#include <es_fe/geom/point.hpp>
#include <es_fe/types.hpp>

#include <es_util/numeric.hpp>

#include <type_traits>

namespace es_fe
{
namespace internal
{
template<class Rect>
Point center(const Rect& rect, Rect_tag)
{
	return {rect.left() + rect.width() / 2, rect.bottom() + rect.height() / 2};
}

// TODO : generic by static/dynamic size
template<class Segment>
Point center(const Segment& segment, Segment_tag)
{
	// return center_static(segment, tag);

	auto&& a = segment.template vertex<0>();
	auto&& b = segment.template vertex<1>();

	return {(a.x() + b.x()) / 2, (a.y() + b.y()) / 2};
}

template<class Triangle>
Point center(const Triangle& triangle, Triangle_tag)
{
	auto circ = triangle.vertex_circ();

	Point c = circ->vertex();
	c += (++circ)->vertex();
	c += (++circ)->vertex();

	return c / 3;
}

template<class Polygon>
Point center(const Polygon& geom, Polygon_tag)
{
	const auto n = geom.n_vertices();

	// TODO : generic
	auto center = geom.vertex(0);
	for (Local_index i = 1; i < n; ++i)
		center += geom.vertex(i);

	center /= n;
	return center;
}

/////

// TODO : check if compiler unwinds loops automatically

// template<class Geometry, Index_t<Geometry>... is>
// Point center_impl(const Geometry& geometry, std::integer_sequence<Index_t<Geometry>, is...>)
// {
// 	return math::mean_of(geometry.template vertex<is>()...);
// }
//
// template<class Geometry>
// Point center_static(const Geometry& geometry)
// {
// 	return center_impl(geometry,
// 		std::make_integer_sequence<Index_t<Geometry>, Geometry::n_vertices()>{});
// }
//
// template<class Geometry>
// Point center_dynamic(const Geometry& geometry)
// {
// 	return math::mean_of_func(geometry.n_vertices(),
// 		[&geometry](auto index) { return geometry.vertex(index); });
// }
} // namespace internal

template<class Geometry>
Point center(const Geometry& geometry)
{
	return internal::center(geometry, typename Geometry::Geometry_tag{});
}
} // namespace es_fe
