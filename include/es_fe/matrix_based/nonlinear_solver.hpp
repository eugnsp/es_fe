#pragma once
#include <es_fe/matrix_based/solution_view.hpp>
#include <es_fe/matrix_based/solver_base.hpp>

#include <es_la/dense.hpp>

#include <cstddef>

namespace es_fe
{
template<class System_, class Linear_solver_>
class Matrix_based_nonlinear_solver : public Matrix_based_solver_base<System_, Linear_solver_>
{
private:
	using Base = Matrix_based_solver_base<System_, Linear_solver_>;

public:
	using Base::Base;

	using Base::system;

	template<class... Args>
	void init(Args&&... args)
	{
		Base::init(std::forward<Args>(args)...);
		step_.resize(system_.n_free_dofs());
	}

	void solve()
	{
		before_solve();

		set_bnd_values();
		linear_solver_.analyze();

		unsigned int n_iter = 0;
		while (true)
		{
			assemble();
			const double residual_norm = rhs_norm2();

			linear_solver_.factorize_solve(rhs_, step_);
			solution_.rows_view(0, step_.size()) += step_;

			const auto step_norm = es_la::norm2(step_);
			const auto solution_norm = es_la::norm2(solution_);

			after_step(n_iter, residual_norm, step_norm, solution_norm);

			if (step_norm / solution_norm < 1e-12 || n_iter > 50)
				break;

			++n_iter;
		}
	}

	template<class System2, class T>
	void set_init_guess(const internal::Solution_base<System2, T>& solution)
	{
		solution_ = solution.values();
	}

	std::size_t memory_size() const
	{
		return Base::memory_size() + step_.memory_size();
	}

protected:
	virtual double rhs_norm2()
	{
		return es_la::norm2(rhs_);
	}

	virtual void set_bnd_values() = 0;
	virtual void assemble() = 0;

	virtual void before_solve()
	{}

	virtual void after_step(
		unsigned int /* n_iter */, double /* residual_norm */, double /* step_norm */, double /* solution_norm */)
	{}

protected:
	using Base::matrix_;
	using Base::rhs_;
	using Base::solution_;

	using Base::linear_solver_;
	using Base::system_;

private:
	es_la::Vector_xd step_;
};
} // namespace es_fe
