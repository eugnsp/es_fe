#pragma once
#include <es_fe/types.hpp>

#include <es_la/dense.hpp>

#include <cstddef>
#include <utility>

namespace es_fe
{
template<class System, std::size_t var>
class Eigen_solution_view
{
public:
	using Mesh = typename System::Mesh;
	using Value = double;

public:
	Eigen_solution_view() = default;

	Eigen_solution_view(const System& system, const unsigned int& n_eigen_values,
		const es_la::Vector_xd& eigen_values, const es_la::Matrix_xd& eigen_vectors) :
		system_(&system),
		n_eigen_values_(&n_eigen_values), eigen_values_(&eigen_values),
		eigen_vectors_(&eigen_vectors)
	{}

	std::size_t size() const
	{
		return *n_eigen_values_;
	}

	Value operator[](std::size_t index) const
	{
		return (*eigen_values_)[index];
	}

	Value operator()(std::size_t i, std::size_t j) const
	{
		return (*eigen_vectors_)(i, j);
	}

	// TODO : make a non-member
	template<class Element, class Quadr>
	Value get(std::size_t index, std::size_t iq,
		const typename System::template Var_dofs<var>& dofs) const
	{
		Value value = 0;
		for (std::size_t k = 0; k < dofs.size(); ++k)
		{
			// HACK
			if (dofs[k].is_free)
			{
				value += es_fe::Element_quadr<Element, Quadr>::basis()(iq, k) *
						 (*eigen_vectors_)(dofs[k].index, index);
			}
		}

		return value;
	}

	template<class Element, class Quadr>
	Value get(std::size_t index, std::size_t iq, const typename Mesh::Cell_view& cell) const
	{
		const auto dofs = system_->dof_mapper().template dofs<var>(cell);
		return get<Element, Quadr>(index, iq, dofs);
	}

	const Mesh& mesh() const
	{
		return system_->mesh();
	}

private:
	const System* system_ = nullptr;
	const unsigned int* n_eigen_values_ = nullptr;
	const es_la::Vector_xd* eigen_values_ = nullptr;
	const es_la::Matrix_xd* eigen_vectors_ = nullptr;
};
} // namespace es_fe
