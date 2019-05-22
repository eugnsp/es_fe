#pragma once
#include <es_fe/types.hpp>

#include <es_la/dense.hpp>

#include <cstddef>
#include <utility>

namespace es_fe
{
template<class System, typename Value>
class Eigen_solution_view
{
public:
	using Mesh = typename System::Mesh;

public:
	Eigen_solution_view(
		const System& system, const es_la::Vector_x<Value>& eigen_values, const es_la::Matrix_x<Value>& eigen_vectors) :
		eigen_values_(eigen_values),
		eigen_vectors_(eigen_vectors), system_(system)
	{}

	std::size_t size() const
	{
		return eigen_values_.size();
	}

	Value operator[](std::size_t index) const
	{
		return eigen_values_[index];
	}

	Value operator()(std::size_t i, std::size_t j) const
	{
		return eigen_vectors_(i, j);
	}

	template<std::size_t var = 0>
	Value at(Vertex_index vertex, std::size_t eigen_pair) const
	{
		typename System::template Var_vertex_dofs<var> dofs;
		system_.dof_mapper().template vertex_dofs<var>(vertex, dofs);
		if (dofs[0].is_free)
			return eigen_vectors_(dofs[0].index, eigen_pair);
		else
			return 0;
	}

	// TODO : make a non-member
	template<class Element, class Quadr, std::size_t var = 0>
	auto at_quadr(std::size_t index, const typename System::template Var_dofs<var>& dofs) const
	{
		assert(index < size());
		es_la::Vector<Value, Quadr::size> vals_at_quadr{};

		for (std::size_t iq = 0; iq < Quadr::size; ++iq)
			for (std::size_t id = 0; id < dofs.size(); ++id)
				if (dofs[id].is_free)
				{
					const auto v = eigen_vectors_(dofs[id].index, index);
					vals_at_quadr[iq] += es_fe::Element_quadr<Element, Quadr>::basis()(iq, id) * v;
				}

		return vals_at_quadr;
	}

	template<class Element, class Quadr, std::size_t var = 0>
	auto at_quadr(std::size_t index, const typename Mesh::Cell_view& cell) const
	{
		const auto dofs = system_.dof_mapper().template dofs<var>(cell);
		return at_quadr<Element, Quadr>(index, dofs);
	}

	const Mesh& mesh() const
	{
		return system_.mesh();
	}

private:
	const es_la::Vector_x<Value>& eigen_values_;
	const es_la::Matrix_x<Value>& eigen_vectors_;

	const System& system_;
};

template<class Element, class Quadr, std::size_t var = 0, class System, class Value>
auto at_quadr(const Eigen_solution_view<System, Value>& solution, std::size_t index,
	const typename System::template Var_dofs<var>& dofs)
{
	return solution.template at_quadr<Element, Quadr, var>(index, dofs);
}

template<class Element, class Quadr, std::size_t var = 0, class System, class Value>
auto at_quadr(
	const Eigen_solution_view<System, Value>& solution, std::size_t index, const typename System::Mesh::Cell_view& cell)
{
	return solution.template at_quadr<Element, Quadr, var>(index, cell);
}
} // namespace es_fe
