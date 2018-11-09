#pragma once
#include <es_fe/types.hpp>
#include <es_fe/mesh/base.hpp>
#include <es_fe/math/quadr.hpp>

#include <es_la/dense.hpp>
#include <es/math/utility.hpp>

#include <cstddef>
#include <type_traits>

namespace es_fe
{
template<class Element, class Quadr>
auto gradients(const la::Matrix_2d& j_it)
{
	constexpr auto n_dofs = Element::n_total_face_dofs;
	constexpr auto basis_grads = Element_quadr<Element, Quadr>::basis_grad();

	la::Matrix<la::Vector_2d, Quadr::size(), n_dofs> grads;
	for (std::size_t i = 0; i < Quadr::size(); ++i)
		for (Local_index r = 0; r < n_dofs; ++r)
			grads(i, r) = j_it * basis_grads(i, r);

	return grads;
}

// TODO : use make_matrix ?
// Returns a local mass matrix
template<class Element, class Quadr, class Func>
auto mass_matrix(Func func, double scale = 1)
{
	constexpr auto n_dofs = Element::n_total_cell_dofs;

	la::Matrix_d<n_dofs, n_dofs> m;
	for (Local_index s = 0; s < n_dofs; ++s)
		for (Local_index r = 0; r < n_dofs; ++r)
			m(r, s) = scale * Quadr::sum([r, s, &func](auto iq) {
						  auto constexpr basis = Element_quadr<Element, Quadr>::basis();
						  return func(iq) * basis(iq, r) * basis(iq, s);
					  });

	return m;
}

// Returns a local mass matrix
template<class Element, class Quadr>
constexpr auto mass_matrix(double scale = 1)
{
	constexpr auto n_dofs = Element::n_total_face_dofs;

	return la::make_matrix<n_dofs, n_dofs>([scale](auto i, auto j) {
		return scale * Quadr::sum([i, j](auto q) {
				   auto constexpr basis = Element_quadr<Element, Quadr>::basis();
				   return basis(q, i) * basis(q, j);
			   });
	});
}

// Returns a local stiffness matrix
template<class Element, class Quadr, class Func, class Grads>
auto stiffness_matrix(const Grads& grads, Func func, double scale = 1)
{
	constexpr auto n_dofs = Element::n_total_face_dofs;

	static_assert(grads.rows() == Quadr::size());
	static_assert(grads.cols() == n_dofs);

	la::Matrix_d<n_dofs, n_dofs> m;
	for (Local_index i = 0; i < n_dofs; ++i)
		for (Local_index j = 0; j < n_dofs; ++j)
			m(i, j) = scale * Quadr::sum([i, j, &grads, &func](auto q) {
						  return func(q) * dot(grads(q, i), grads(q, j));
					  });

	return m;
}

// Returns a local stiffness matrix
template<class Element, class Quadr, class Func, class Grads>
auto stiffness_matrix(const Grads& grads, Func func, la::Matrix_2d eps, double scale = 1)
{
	constexpr auto n_dofs = Element::n_dofs();

	static_assert(grads.rows() == Quadr::size());
	static_assert(grads.cols() == n_dofs);

	la::Matrix_d<n_dofs, n_dofs> m;
	for (Local_index i = 0; i < n_dofs; ++i)
		for (Local_index j = 0; j < n_dofs; ++j)
			m(i, j) = scale * Quadr::sum([i, j, &grads, &func, &eps](auto q) {
						  return func(q) * dot(grads(q, i), eps * grads(q, j));
					  });

	return m;
}

// Returns a local stiffness matrix
template<class Element, class Quadr, class Grads>
auto stiffness_matrix(const Grads& grads, double scale = 1)
{
	return stiffness_matrix<Element, Quadr>(grads, math::One{}, scale);
}

// Returns a local stiffness matrix
template<class Element, class Quadr, class Grads>
auto stiffness_matrix(const Grads& grads, la::Matrix_2d eps, double scale = 1)
{
	return stiffness_matrix<Element, Quadr>(grads, math::One{}, eps, scale);
}

// Returns a local load vector
template<class Element, class Quadr, class Func = math::One<int>>
auto load_vector(Func func = math::One<int>{}, double scale = 1)
{
	constexpr auto n_dofs = Element::n_total_face_dofs;

	return la::make_vector<n_dofs>([&func, scale](auto i) {
		return scale * Quadr::sum([i, &func](Local_index q) {
				   constexpr auto basis = Element_quadr<Element, Quadr>::basis();
				   return func(q) * basis(q, i);
			   });
	});
}
} // namespace es_fe
