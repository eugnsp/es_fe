#pragma once
#include <es_fe/geom/point.hpp>
#include <es_fe/geom/points.hpp>
#include <es_fe/types.hpp>
#include <cassert>
#include <vector>
#include <utility>

namespace es_fe
{
// A linestring (a sequence of points and line segments connecting them)
class Linestring : public Points<invalid_local_index>
{
public:
	using Geometry_tag = Linestring_tag;

public:
	explicit Linestring(std::initializer_list<Point> vertices) : Points(vertices)
	{
		assert(n_vertices() > 1);
	}

	explicit Linestring(std::vector<Point> vertices) : Points(std::move(vertices))
	{
		assert(n_vertices() > 1);
	}
};
} // namespace es_fe
