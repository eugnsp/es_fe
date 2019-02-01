#pragma once
#include <es_fe/matrix_based/solution_view.hpp>

#include <es_la/dense.hpp>

#include <cstddef>
#include <memory>

namespace es_fe
{
template<class System_, class Linear_solver_>
class Matrix_based_solver
{
public:
	using System = System_;
	using Mesh = typename System::Mesh;

private:
	// template<std::size_t var>
	// using Solution_view_t = Solution_view<System, var>;

	// template<std::size_t var, class Mesh_el_tag>
	// using Solution_view_t2 = Solution_view2<System, var, Mesh_el_tag>;

	template<std::size_t var>
	using My_solution_view = Solution_view<Matrix_based_solver, var>;

	template<class Solver, std::size_t var>
	friend class Solution_view;

public:
	Matrix_based_solver(const Mesh& mesh) : system_(mesh)
	{}

	template<class... Args>
	void init(Args&&... args)
	{
		system_.init(std::forward<Args>(args)...);

		const auto n = system_.n_dofs();
		const auto nf = system_.n_free_dofs();

		solution_.resize(n);
		matrix_.resize(nf, nf);
		rhs_.resize(nf);
	}

	void solve()
	{
		matrix_.zero();
		rhs_.zero();

		before_solve();

		set_bnd_values();
		assemble();
		after_assemble();

		linear_solver_.analyze_factorize_solve(matrix_, rhs_, solution_);

		after_solve();
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

	template<std::size_t var = 0>
	My_solution_view<var> solution_view() const
	{
		return My_solution_view<var>{*this};
	}

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
		// return solution_.memory_size() + rhs_.memory_size() + matrix_.memory_size() +
		// system_.memory_size();
		return 0;
	}

protected:
	virtual void set_bnd_values() = 0;

	virtual void before_solve()
	{}

	virtual void after_solve()
	{}

	virtual void after_assemble()
	{}

	virtual void assemble() = 0;

protected:
	la::Vector_xd solution_;
	typename Linear_solver_::Sparse_matrix matrix_;
	la::Vector_xd rhs_;

	Linear_solver_ linear_solver_;
	System system_;
};
} // namespace es_fe
