#pragma once
#include <es_fe/geom/algorithm/distance.hpp>
#include "point.hpp"
#include <es_util/numeric.hpp>
//#include "math/LA/VectorFunction.h"
#include <algorithm>
#include <cmath>

namespace es_fe
{
// Basic geometric functions

inline double distance_squared(const Point& a, const Point& b)
{
	const auto dx = a.x() - b.x();
	const auto dy = a.y() - b.y();
	return dx * dx + dy * dy;
}

/** Checks whether four points form a rectangle.
 *  Precondition: points should be order anti-clockwise. */
bool isAlignedRect(const Point&, const Point&, const Point&, const Point&);

// Checks if the point lies on the line segment (a, b)
bool is_point_on_line_segment(const Point&, const Point& a, const Point& b);

double GetDistanceToLineSegment(const Point& point, const Point& a, const Point& b);

enum class Side
{
	ON_THE_LINE,
	ON_THE_LEFT,
	ON_THE_RIGHT
};

// Computes which side of the directed line (a -> b) the point is located at
Side which_side(const Point&, const Point& a, const Point& b);

// Computes the intersection point between the line segment [x1, x2] and
// the line passing through points (y1) and (y2)
Point GetSegmentAndLineIntersectionPoint(const Point& _x1, const Point& _x2, const Point& _y1,
										 const Point& _y2);

// Clips simple by the line passing through points (y1) and (y2)
// void ClipSimpleByLine(SimplePolygon& _out, const SimplePolygon& _in, const Point& _y1, const
// Point& _y2);

// Functions related to triangles

// Point circumcenter(const Point&, const Point&, const Point&);
// double circumradius(const Point&, const Point&, const Point&);

// 	inline double signed_area(const Point& a, const Point& b, const Point& c)
// 	{
// 		return math::det(a.x(), a.y(), 1., b.x(), b.y(), 1., c.x(), c.y(), 1.) / 2;
// 	}
//
// 	inline double area(const Point& a, const Point& b, const Point& c)
// 	{
// 		return std::abs(signed_area(a, b, c));
// 	}

// Diameter of a triangle is the maximum distance between any two vertices
inline double diameter(const Point& a, const Point& b, const Point& c)
{
	return std::max({distance(b, c), distance(c, a), distance(a, b)});
}
} // namespace es_fe
