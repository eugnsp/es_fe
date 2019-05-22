#pragma once
#include <es_fe/matrix_based/solution.hpp>
#include <es_fe/matrix_based/solution_view.hpp>

#include <es_la/dense.hpp>

#include <cstddef>

namespace es_fe
{
template<class System_, class Linear_solver>
class Matrix_based_solver_base
{
	// template<class Solver, std::size_t var>
	// friend class Solution_view;

public:
	using System = System_;
	using Mesh = typename System::Mesh;

	using Value = typename Linear_solver::Sparse_matrix::Value;
	using Solution = es_fe::Solution<System, Value>;
	using Solution_view = es_fe::Solution_view<System, Value>;

public:
	Matrix_based_solver_base(const Mesh& mesh) : linear_solver_(matrix_), system_(mesh)
	{}

	template<class... Args>
	void init(Args&&... args)
	{
		system_.init(std::forward<Args>(args)...);

		const auto n = system_.n_dofs();
		const auto nf = system_.n_free_dofs();

		matrix_.resize(nf, nf);
		solution_.resize(n);
		rhs_.resize(nf);
	}

	System& system()
	{
		return system_;
	}

	const System& system() const
	{
		return system_;
	}

	const Mesh& mesh() const
	{
		return system_.mesh();
	}

	Solution solution() const
	{
		return {system_, solution_};
	}

	Solution_view solution_view() const
	{
		return {system_, solution_};
	}

	// template<std::size_t var = 0>
	// auto solution_view() const
	// {
	// 	return Solution_view<Matrix_based_solver_base, var>{*this};
	// }

	// 	template<std::size_t var>
	// Solution_view_t<var> solution_view() const
	// {
	// 	debug_check_var_index<var>();
	// 	return {*this};
	// }

	// template<std::size_t var, class Mesh_el_tag>
	// auto solution_view2() const
	// {
	// 	debug_check_var_index<var>();
	// 	return Solution_view_t2<var, Mesh_el_tag>{*this};
	// }

	std::size_t memory_size() const
	{
		return solution_.memory_size() + rhs_.memory_size() + matrix_.memory_size() + system_.memory_size();
	}

protected:
	es_la::Vector_x<Value> solution_;
	es_la::Vector_x<Value> rhs_;
	typename Linear_solver::Sparse_matrix matrix_;

	Linear_solver linear_solver_;
	System system_;
};
} // namespace es_fe
