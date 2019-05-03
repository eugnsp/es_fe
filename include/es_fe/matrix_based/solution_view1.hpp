#pragma once
#include <es_fe/types.hpp>

#include <es_la/dense.hpp>

#include <cstddef>
#include <utility>

namespace es_fe
{
template<class System, std::size_t var>
class Solution_view1
{
public:
	using Mesh = typename System::Mesh;
	using Value = double;

public:
	Solution_view1() = default;

	Solution_view1(const System& system, es_la::Vector_xd& solution) :
		system_(&system), solution_(&solution)
	{}

	Value operator[](es_fe::Vertex_index vertex) const
	{
		typename System::template Var_vertex_dofs<var> dofs;
		system_->dof_mapper().template vertex_dofs<var>(vertex, dofs);

		return (*solution_)[dofs[0].index];
	}

	// TODO : make a non-member
	template<class Element, class Quadr>
	Value get(std::size_t iq, const typename Mesh::Cell_view& cell) const
	{
		const auto dofs = system_->dof_mapper().template dofs<var>(cell);
		return get<Element, Quadr>(iq, dofs);
	}

	// TODO : make a non-member
	template<class Element, class Quadr>
	Value get(std::size_t iq, const typename System::template Var_dofs<var>& dofs) const
	{
		Value value = 0;
		for (std::size_t k = 0; k < dofs.size(); ++k)
		{
			const auto s = (*solution_)[dofs[k].index];
			value += es_fe::Element_quadr<Element, Quadr>::basis()(iq, k) * s;
		}

		return value;
	}

	const Mesh& mesh() const
	{
		return system_->mesh();
	}

	es_la::Vector_xd& raw()
	{
		return *solution_;
	}

	const es_la::Vector_xd& raw() const
	{
		return *solution_;
	}

	Solution_view1 clone(es_la::Vector_xd& solution)
	{
		return {*system_, solution};
	}

private:
	const System* system_ = nullptr;
	es_la::Vector_xd* solution_ = nullptr;
};
} // namespace es_fe
