#pragma once
#include <es/fe/types.hpp>
#include <es/fe/element/linear_element.hpp>
#include <es/fe/element/triangular_element.hpp>
#include <es/fe/element/lagrange/lagrange_base_1.hpp>
#include <es/fe/element/lagrange/lagrange_base_2.hpp>
#include <es/fe/element/lagrange/element_quadr_lagrange_1.hpp>
#include <es/fe/element/lagrange/element_quadr_lagrange_2.hpp>

namespace fe
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

//////////////////////////////////////////////////////////////////////////

template<Local_index order>
class Discontinuous_lagrange<order, 1> :
	public internal::Linear_element<0, order + 1>,
	public internal::Lagrange_base_1<order>
{ };

template<Local_index order>
class Discontinuous_lagrange<order, 2> :
	public internal::Triangular_element<0, 0, (order + 1) * (order + 2) / 2>,
	public internal::Lagrange_base_2<order>
{ };
}