#pragma once
#include <es_fe/geom/point.hpp>
#include <es_fe/geom/points.hpp>
#include <es_fe/types.hpp>

#include <es_util/algorithm.hpp>
#include <es_util/array.hpp>

#include <cassert>
#include <vector>
#include <utility>

namespace fe
{
template<unsigned int n>
auto turtle(const Point (&points)[n])
{
	auto pts = es::util::to_array(points);
	es::util::for_each_pair(pts.begin(), pts.end(), [](const auto& p1, auto& p2) { p2 += p1; });

	return pts;
}

// A simple polygon (a sequence of points defining an exterior bounding
// ring with non-intersecting sides) with a CCW orientation
template<unsigned int n>
class Polygon : public Points<n>
{
public:
	using Geometry_tag = Polygon_tag;

private:
	static_assert(n > 2, "Degenerate polygons are not allowed");
	using Base = Points<n>;

public:
	using Base::Base;
};

template<>
class Polygon<dynamic> : public Points<dynamic>
{
public:
	using Geometry_tag = Polygon_tag;

private:
	using Base = Points<dynamic>;

public:
	explicit Polygon(std::initializer_list<Point> vertices) : Base(vertices)
	{
		assert(n_vertices() > 2);
	}

	explicit Polygon(std::vector<Point> vertices) : Base(std::move(vertices))
	{
		assert(n_vertices() > 2);
	}
};
} // namespace fe