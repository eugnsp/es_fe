#pragma once
#include <es_fe/types.hpp>

#include <es_la/dense.hpp>

#include <cassert>
#include <cstddef>
#include <type_traits>
#include <utility>

namespace es_fe
{
namespace internal
{
template<class System, class T>
class Solution_base
{
private:
	using Value = es_la::Value_type<T>;

public:
	Solution_base(const System& system, const T& values) : values_(values), system_(system)
	{}

	///////////////////////////////////////////////////////////////////////
	//* Solution values access */

	const T& values() const
	{
		return values_;
	}

	template<class Element, class Quadr, std::size_t var = 0>
	auto at_quadr(const typename System::template Var_dofs<var>& dofs) const
	{
		es_la::Vector<Value, Quadr::size> vals_at_quadr{};

		for (std::size_t iq = 0; iq < Quadr::size; ++iq)
			for (std::size_t id = 0; id < dofs.size(); ++id)
			{
				const auto v = values_[dofs[id].index];
				vals_at_quadr[iq] += Element_quadr<Element, Quadr>::basis()(iq, id) * v;
			}

		return vals_at_quadr;
	}

	auto& operator[](Vertex_index vertex) const
	{
		constexpr auto var = 0;

		typename System::template Var_vertex_dofs<var> dofs;
		system_.dof_mapper().template vertex_dofs<var>(vertex, dofs);

		return values_[dofs[0].index];
	}

	// TODO : make a non-member
	template<class Element, class Quadr>
	auto get(std::size_t iq, const typename System::Mesh::Cell_view& cell) const
	{
		constexpr auto var = 0;
		const auto dofs = system_.dof_mapper().template dofs<var>(cell);
		return get<Element, Quadr>(iq, dofs);
	}

	// TODO : make a non-member
	template<class Element, class Quadr, std::size_t var = 0>
	auto get(std::size_t iq, const typename System::template Var_dofs<var>& dofs) const
	{
		Value value = 0;
		for (std::size_t k = 0; k < dofs.size(); ++k)
		{
			const auto s = values_[dofs[k].index];
			value += es_fe::Element_quadr<Element, Quadr>::basis()(iq, k) * s;
		}

		return value;
	}

	const System& system() const
	{
		return system_;
	}

protected:
	T values_;
	const System& system_;
};
}

template<class Element, class Quadr, std::size_t var = 0, class System, class T>
auto at_quadr(const internal::Solution_base<System, T>& solution, const typename System::template Var_dofs<var>& dofs)
{
	return solution.template at_quadr<Element, Quadr, var>(dofs);
}
} // namespace es_fe::internal
