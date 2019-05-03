#pragma once
#include <es_fe/element/lagrange/element_quadr_lagrange.hpp>
#include <es_fe/element/lagrange/lagrange_base_1.hpp>
#include <es_fe/element/lagrange/lagrange_base_2.hpp>
#include <es_fe/element/linear_element.hpp>
#include <es_fe/element/triangular_element.hpp>
#include <es_fe/types.hpp>

namespace es_fe
{
template<Local_index order>
class Lagrange<order, 1> :
	public internal::Linear_element<1, order - 1>,
	public internal::Lagrange_base_1<order>
{
	static_assert(order > 0);
};

template<Local_index order>
class Lagrange<order, 2> :
	public internal::Triangular_element<1, order - 1, (order - 1) * (order - 2) / 2>,
	public internal::Lagrange_base_2<order>
{
	static_assert(order > 0);
};

template<Local_index order>
class Discontinuous_lagrange<order, 1> :
	public internal::Linear_element<0, order + 1>,
	public internal::Lagrange_base_1<order>
{};

template<Local_index order>
class Discontinuous_lagrange<order, 2> :
	public internal::Triangular_element<0, 0, (order + 1) * (order + 2) / 2>,
	public internal::Lagrange_base_2<order>
{};
} // namespace es_fe
