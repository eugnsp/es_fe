#pragma once
#include <esf/types.hpp>

#include <esl/dense.hpp>

namespace es_fe
{
template<es_fe::Local_index order, es_fe::Local_index dim, class Quadr>
class Element_quadr<es_fe::Lagrange<order, dim>, Quadr>
{
private:
	using Element = es_fe::Lagrange<order, dim>;
	static constexpr auto n_points = Quadr::size;
	static constexpr auto n_dofs = Element::n_total_cell_dofs;

public:
	// Returns the values of basis functions at the quadrature points
	// as the matrix of the size (# quadr. points) x (# dofs)
	static constexpr auto basis()
	{
		return esl::make_matrix<n_points, n_dofs>([](
			auto quadr, auto dof) constexpr { return Element::basis(dof, Quadr::point(quadr)); });
	}

	// Returns the values of gradients of basis functions at the quadrature points
	// as the matrix of the size (# quadr. points) x (# dofs)
	static constexpr auto basis_grad()
	{
		return esl::make_matrix<n_points, n_dofs>([](auto quadr, auto dof) constexpr {
			return Element::basis_grad(dof, Quadr::point(quadr));
		});
	}
};

template<es_fe::Local_index order, es_fe::Local_index dim, class Quadr>
class Element_quadr<es_fe::Discontinuous_lagrange<order, dim>, Quadr> :
	public Element_quadr<es_fe::Lagrange<order, dim>, Quadr>
{};
} // namespace es_fe
