#pragma once
#include <es_fe/types.hpp>
#include <es/math/const.hpp>
#include <es_util/array.hpp>

namespace es_fe::internal
{
struct Gauss_point
{
	double weight;
	double x;
};

template<Local_index t_order>
struct Gauss_data
{};

template<>
struct Gauss_data<0>
{
	static constexpr auto points = es_util::make_array(Gauss_point{2, 0});
};

template<>
struct Gauss_data<1> : Gauss_data<0>
{};

template<>
struct Gauss_data<2>
{
	static constexpr auto points = es_util::make_array(Gauss_point{1, -math::sqrt_three / 3},
													   Gauss_point{1, math::sqrt_three / 3});
};

template<>
struct Gauss_data<3>
{
	static constexpr auto points =
		es_util::make_array(Gauss_point{5. / 9, -7.745966692414834e-1}, Gauss_point{8. / 9, 0},
							Gauss_point{5. / 9, 7.745966692414834e-1});
};

template<>
struct Gauss_data<4>
{
	static constexpr auto points =
		es_util::make_array(Gauss_point{3.4785484513745391e-1, -8.6113631159405302e-1},
							Gauss_point{6.5214515486254621e-1, -3.3998104358485626e-1},
							Gauss_point{6.5214515486254621e-1, 3.3998104358485626e-1},
							Gauss_point{3.4785484513745391e-1, 8.6113631159405291e-1});
};

template<>
struct Gauss_data<5>
{
	static constexpr auto points = es_util::make_array(
		Gauss_point{2.3692688505618870e-1, -9.0617984593866396e-1},
		Gauss_point{4.7862867049936653e-1, -5.3846931010568289e-1}, Gauss_point{128. / 225, 0},
		Gauss_point{4.7862867049936653e-1, 5.3846931010568300e-1},
		Gauss_point{2.3692688505618870e-1, 9.0617984593866385e-1});
};
} // namespace es_fe::internal