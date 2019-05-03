#pragma once
#include <es_fe/geometry/algorithm/distance.hpp>
#include <es_fe/types.hpp>

#include <es_util/algorithm.hpp>

namespace es_fe
{
namespace internal
{
template<class Segment>
double length(const Segment& segment, Segment_tag)
{
	return distance(segment.vertex(0), segment.vertex(1));
}

template<class Linestring>
double length(const Linestring& linestring, Linestring_tag)
{
	double l = 0;
	es_util::for_each_pair(linestring.begin_vertex(), linestring.end_vertex(),
		[&l](const auto& v1, const auto& v2) { l += distance(v1, v2); });

	return l;
}
} // namespace internal

template<class Geometry>
double length(const Geometry& geometry)
{
	return internal::length(geometry, typename Geometry::Geometry_tag{});
}
} // namespace es_fe
