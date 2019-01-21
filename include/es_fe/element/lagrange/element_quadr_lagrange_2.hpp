#pragma once
#include <es_fe/types.hpp>

#include <es_la/dense.hpp>

namespace es_fe
{
template<Local_index order, class Quadr>
class Element_quadr<Lagrange<order, 2>, Quadr>
{
private:
	using Element = Lagrange<order, 2>;
	static constexpr auto n_points = Quadr::size();
	static constexpr auto n_dofs = Element::n_total_face_dofs;

public:
	// Returns the values of basis functions at the quadrature points
	// in the matrix of the size (# quadr. points) x (# dofs)
	static constexpr la::Matrix_d<n_points, n_dofs> basis()
	{
		return la::make_matrix<n_points, n_dofs>([](Local_index quadr, Local_index dof) constexpr {
			return Element::basis(dof, Quadr::point(quadr));
		});
	}

	// Returns the values of gradients of basis functions at the quadrature points
	// in the matrix of the size (# quadr. points) x (# dofs)
	static constexpr la::Matrix<la::Vector_2d, n_points, n_dofs> basis_grad()
	{
		return la::make_matrix<n_points, n_dofs>([](Local_index quadr, Local_index dof) constexpr {
			return Element::basis_grad(dof, Quadr::point(quadr));
		});
	}
};

template<Local_index order, class Quadr>
class Element_quadr<Discontinuous_lagrange<order, 2>, Quadr> :
	Element_quadr<Lagrange<order, 2>, Quadr>
{};
} // namespace es_fe
