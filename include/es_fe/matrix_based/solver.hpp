#pragma once
#include <es_fe/matrix_based/solver_base.hpp>
#include <es_fe/matrix_based/solution_view.hpp>

#include <es_la/dense.hpp>

#include <cstddef>
#include <memory>

namespace es_fe
{
template<class System_, class Linear_solver_>
class Matrix_based_solver : public Matrix_based_solver_base<System_, Linear_solver_>
{
private:
	using Base = Matrix_based_solver_base<System_, Linear_solver_>;
	// template<std::size_t var>
	// using Solution_view_t = Solution_view<System, var>;

	// template<std::size_t var, class Mesh_el_tag>
	// using Solution_view_t2 = Solution_view2<System, var, Mesh_el_tag>;

	template<std::size_t var>
	using My_solution_view = Solution_view<Matrix_based_solver, var>;

	template<class Solver, std::size_t var>
	friend class Solution_view;

public:
	using Base::Base;

	template<class... Args>
	void init(Args&&... args)
	{
		Base::init(std::forward<Args>(args)...);
	}

	void solve()
	{
		matrix_.zero();
		rhs_ = 0;

		before_solve();

		set_bnd_values();
		assemble();
		after_assemble();

		linear_solver_.analyze_factorize_solve(rhs_, solution_);

		after_solve();
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
	using Base::solution_;
	using Base::rhs_;
	using Base::matrix_;
	using Base::linear_solver_;
};
} // namespace es_fe
