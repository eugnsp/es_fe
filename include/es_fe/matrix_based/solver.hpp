#pragma once
#include <es/fe/matrix_based/seq_assembler.hpp>

#include <memory>

namespace fe
{
template<class Derived_solver_, class System_, class Linear_solver_, class Assembler_ = Seq_assembler>
class Matrix_based_solver
{
public:
	using System = System_;
	using Mesh = typename System::Mesh;

public:
	Matrix_based_solver(const Mesh& mesh) :
		system_(mesh)
	{ }

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
		assembler_.assemble(system_, self());
		after_assemble();


		linear_solver_.analyze_factorize_solve(matrix_, rhs_, solution_);

		after_solve();
	}

	Assembler_& assembler()
	{
		return assembler_;
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

	std::size_t memory_size() const
	{
		//return solution_.memory_size() + rhs_.memory_size() + matrix_.memory_size() + system_.memory_size();
		return 0;
	}

private:
	Derived_solver_& self()
	{
		return static_cast<Derived_solver_&>(*this);
	}

protected:
	virtual void set_bnd_values() = 0;

	virtual void before_solve()
	{ }

	virtual void after_solve()
	{ }

	virtual void after_assemble()
	{ }

protected:
	la::Vector_xd solution_;
	typename Linear_solver_::Sparse_matrix matrix_;
	la::Vector_xd rhs_;

	Assembler_ assembler_;
	Linear_solver_ linear_solver_;
	System system_;
};
}