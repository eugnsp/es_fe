#pragma once
#include <es_fe/matrix_based/solution_view.hpp>
#include <es_fe/matrix_based/solution_view1.hpp>

#include <es_la/dense.hpp>

#include <cstddef>
#include <memory>

#include <iostream>

namespace es_fe
{
template<class System_, class Linear_solver_>
class Matrix_based_nonlinear_solver
{
public:
	using System = System_;
	using Mesh = typename System::Mesh;

public:
	// template<std::size_t var>
	// using Solution_view_t = Solution_view<System, var>;

	// template<std::size_t var, class Mesh_el_tag>
	// using Solution_view_t2 = Solution_view2<System, var, Mesh_el_tag>;

	template<std::size_t var>
	using Solution_view = Solution_view1<System, var>;

	template<class Solver, std::size_t var>
	friend class Solution_view1;

public:
	Matrix_based_nonlinear_solver(const Mesh& mesh) : linear_solver_(matrix_), system_(mesh)
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
		step_.resize(nf);
	}

	void solve()
	{
		set_bnd_values();

		linear_solver_.analyze();

		unsigned int it = 0;
		while (true)
		{
			assemble();
			const double residual_norm = rhs_norm2();

			linear_solver_.factorize_solve(rhs_, step_);
			solution_.view(0, step_.size(), 0, 1) += step_;

			const auto step_norm = es_la::norm2(step_);
			std::cout << step_norm << ' ' << residual_norm << std::endl;

			if (step_norm < 1e-7 || it > 50)
				break;

			++it;
		}
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
	Solution_view<var> solution_view()
	{
		return Solution_view<var>{system(), solution_};
	}

	template<std::size_t var = 0>
	Solution_view<var> solution_view(const es_la::Vector_xd& solution) const
	{
		return Solution_view<var>{system(), solution};
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
	virtual double rhs_norm2()
	{
		return es_la::norm2(rhs_);
	}

	virtual void set_bnd_values() = 0;
	virtual void assemble() = 0;

protected:
	es_la::Vector_xd solution_;
	typename Linear_solver_::Sparse_matrix matrix_;
	es_la::Vector_xd rhs_;
	es_la::Vector_xd step_;

	Linear_solver_ linear_solver_;
	System system_;
};
} // namespace es_fe
