#pragma once
#include <es_fe/geometry/algorithm/equals.hpp>
#include <es_fe/geometry/compare.hpp>
#include <es_fe/types.hpp>

#include <es_la/dense.hpp>
#include <es_util/numeric.hpp>

#include <cassert>
#include <cmath>
#include <cstddef>
#include <iosfwd>

namespace es_fe
{
class Point2 : public es_la::Vector_2d
{
public:
	using Geometry_tag = Point2_tag;

private:
	using Base = es_la::Vector_2d;

public:
	using Base::Base;

	Point2(double x, double y) : Base({x, y})
	{}

	double& x()
	{
		return (*this)[0];
	}

	double& y()
	{
		return (*this)[1];
	}

	double x() const
	{
		return (*this)[0];
	}

	double y() const
	{
		return (*this)[1];
	}
};

// inline Point operator*(Point p, double scalar)
// {
// 	p *= scalar;
// 	return p;
// }
//
// inline Point operator*(double scalar, const Point& p)
// {
// 	return p * scalar;
// }
//
// inline Point operator/(Point p, double scalar)
// {
// 	p /= scalar;
// 	return p;
// }

// Lexicographical ordering of two points
inline bool operator<(const Point2& lhs, const Point2& rhs)
{
	if (is_geom_less(lhs.x(), rhs.x()))
		return true;

	if (is_geom_greater(lhs.x(), rhs.x()))
		return false;

	if (is_geom_less(lhs.y(), rhs.y()))
		return true;

	return false;
}

// inline bool isPointInRefTriangle(const Point& p)
// {
// 	return p.x() > -PHYS_MIN_POINT_DISTANCE
// 		&& p.y() > -PHYS_MIN_POINT_DISTANCE
// 		&& p.x() + p.y() < 1 + PHYS_MIN_POINT_DISTANCE;
// }

inline bool operator==(const es_fe::Point2& a, const es_fe::Point2& b)
{
	return equals(a, b);
}

/** Returns the dot product of two points @f$ a_x b_x + a_y b_y @f$. */
inline double operator*(const es_fe::Point2& a, const es_fe::Point2& b)
{
	return a.x() * b.x() + a.y() * b.y();
}

// Returns the cross product of two points, ax * by - ay * bx
inline double operator^(const es_fe::Point2& a, const es_fe::Point2& b)
{
	return es_util::det(a.x(), a.y(), b.x(), b.y());
}

std::ostream& operator<<(std::ostream& out, const es_fe::Point2& p);
} // namespace es_fe