#pragma once
#include <es_fe/matrix_based/eigen_solution_view.hpp>

#include <es_la/dense.hpp>

#include <cstddef>
#include <memory>

#include <es_la/sparse.hpp>

namespace es_fe
{
template<class System_, class Eigen_solver_>
class Matrix_based_eigen_solver
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
	using Solution_view = Eigen_solution_view<System, var>;

	// template<class Solver, std::size_t var>
	// friend class Solution_view;

public:
	Matrix_based_eigen_solver(const Mesh& mesh) : eigen_solver_(matrix_a_, matrix_b_), system_(mesh)
	{}

	template<class... Args>
	void init(Args&&... args)
	{
		system_.init(std::forward<Args>(args)...);

		const auto nf = system_.n_free_dofs();

		matrix_a_.resize(nf, nf);
		matrix_b_.resize(nf, nf);
	}

	void solve()
	{
		before_solve();

		//		set_bnd_values();
		assemble();
		after_assemble();

		// linear_solver_.analyze_factorize_solve(matrix_, rhs_, solution_);

		auto dim = eigen_space_dim();
		eigen_values_.resize(dim);
		eigen_vectors_.resize(matrix_a_.rows(), dim);

		auto range = eigen_values_range();

		{
			// sparse_matrix_t a;
			// auto z = matrix_a_.row_indices();
			// auto v = matrix_a_.values();
			// auto r1 = mkl_sparse_d_create_csr(&a, SPARSE_INDEX_BASE_ZERO, matrix_a_.rows(),
			// 	matrix_a_.cols(), (MKL_INT*)(z), (MKL_INT*)(matrix_a_.row_indices()) + 1,
			// 	(MKL_INT*)(matrix_a_.col_indices()), (double*)v);

			// sparse_matrix_t b;
			// auto r3 = mkl_sparse_convert_csr(a, SPARSE_OPERATION_TRANSPOSE, &b);

			// sparse_index_base_t indexing;
			// MKL_INT rows;
			// MKL_INT cols;
			// MKL_INT* rows_start;
			// MKL_INT* rows_end;
			// MKL_INT* col_indx;
			// double* values;
			// auto r2 = mkl_sparse_d_export_csr(
			// 	b, &indexing, &rows, &cols, &rows_start, &rows_end, &col_indx, &values);

			// mkl_sparse_destroy(a);
			// mkl_sparse_destroy(b);
		}

		while (true)
		{
			if (eigen_solver_.solve(eigen_vectors_, eigen_values_, range))
				break;

			dim += 2;
			dim *= 1.2;
		}

		n_eigen_values_ = eigen_solver_.n_eigen_values();

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
	Solution_view<var> solution_view() const
	{
		return Solution_view<var>{system(), n_eigen_values_, eigen_values_, eigen_vectors_};
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
	// virtual void set_bnd_values() = 0;

	virtual std::pair<double, double> eigen_values_range() const = 0;

	virtual unsigned int eigen_space_dim() const = 0;

	virtual void before_solve()
	{}

	virtual void after_solve()
	{}

	virtual void after_assemble()
	{}

	virtual void assemble() = 0;

protected:
	es_la::Vector_xd eigen_values_;
	es_la::Matrix_xd eigen_vectors_;
	std::size_t n_eigen_values_;

	typename Eigen_solver_::Sparse_matrix matrix_a_;
	typename Eigen_solver_::Sparse_matrix matrix_b_;

	Eigen_solver_ eigen_solver_;
	System system_;
};
} // namespace es_fe
