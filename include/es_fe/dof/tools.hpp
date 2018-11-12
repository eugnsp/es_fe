#pragma once
#include <utility>

namespace es_fe
{
template<class Sparse_matrix, class System>
void compute_and_set_sparsity_pattern(const System& system, Sparse_matrix& matrix)
{
	using Symmetry = typename Sparse_matrix::Symmetry_tag;

	auto pattern = system.template sparsity_pattern<Symmetry>();
	matrix.set_sparsity_pattern(std::move(pattern));
}

template<class Sparse_matrix, class System, class Coupling_fn>
void compute_and_set_sparsity_pattern(const System& system, Coupling_fn&& coupling_fn,
									  Sparse_matrix& matrix)
{
	using Symmetry = typename Sparse_matrix::Symmetry_tag;

	auto pattern =
		system.template sparsity_pattern<Symmetry>(std::forward<Coupling_fn>(coupling_fn));
	matrix.set_sparsity_pattern(std::move(pattern));
}
} // namespace es_fe