#pragma once
#include <es_fe/types.hpp>
#include <es_fe/geom/point.hpp>

#include <es_util/array.hpp>

#include <algorithm>
#include <array>
#include <cassert>
#include <initializer_list>
#include <vector>
#include <utility>

namespace es_fe
{
namespace internal
{
template<Local_index n>
class Points_base
{
public:
	using Geometry_tag = Points_tag;

public:
	const Point& vertex(Local_index index) const
	{
		return vertices_[index];
	}

	// Returns the first vertex
	const Point& first() const
	{
		return vertices_.front();
	}

	// Returns the last vertex
	const Point& last() const
	{
		return vertices_.back();
	}

	auto begin_vertex() const
	{
		return vertices_.begin();
	}

	auto end_vertex() const
	{
		return vertices_.end();
	}

protected:
	Points_base() = default;

	template<typename Points>
	Points_base(Points&& points) : vertices_(std::forward<Points>(points))
	{}

	// TODO
	Points_base(std::initializer_list<Point> points)
	{
		if constexpr (n == invalid_local_index)
			vertices_.resize(points.size());
		std::copy(points.begin(), points.end(), vertices_.begin());
	}

private:
	using List =
		std::conditional_t<n == invalid_local_index, std::vector<Point>, std::array<Point, n>>;

protected:
	List vertices_;
};
} // namespace internal

// A collection of points
template<Local_index n>
class Points : public internal::Points_base<n>
{
private:
	using Base = internal::Points_base<n>;

public:
	Points() = default;

	explicit Points(const Point (&vertices)[n]) : Base(es_util::to_array(vertices))
	{}

	explicit Points(const std::array<Point, n>& vertices) : Base(vertices)
	{}

	explicit Points(std::initializer_list<Point> vertices) : Base(vertices)
	{}

	static constexpr Local_index n_vertices()
	{
		return n;
	}
};

template<>
class Points<invalid_local_index> : public internal::Points_base<invalid_local_index>
{
private:
	using Base = internal::Points_base<invalid_local_index>;

public:
	Points() = default;

	explicit Points(std::initializer_list<Point> vertices) : Base(vertices)
	{}

	explicit Points(std::vector<Point> vertices) : Base(std::move(vertices))
	{}

	Local_index n_vertices() const
	{
		return static_cast<Local_index>(vertices_.size());
	}
};
} // namespace es_fe
