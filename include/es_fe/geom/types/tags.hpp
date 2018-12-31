#pragma once

namespace es_fe
{
inline constexpr auto dynamic = static_cast<unsigned int>(-1);

namespace internal
{
struct Polygon_tag
{};
struct Linestring_tag
{};
struct Rect_tag
{};
struct Triangle_tag : Polygon_tag
{};
struct Segment_tag
{};
struct Points_tag
{};
struct Point_tag
{};
} // namespace internal
} // namespace es_fe
