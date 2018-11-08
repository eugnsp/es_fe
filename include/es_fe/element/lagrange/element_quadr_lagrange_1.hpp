#pragma once
#include <es_fe/types.hpp>

#include <es_la/dense.hpp>

namespace es_fe
{
template<Local_index order, class Quadr>
class Element_quadr<Lagrange<order, 1>, Quadr>
{
private:
	using Element = Lagrange<order, 1>;
	static constexpr auto n_points = Quadr::size();
	static constexpr auto n_dofs = Element::n_total_edge_dofs;

public:
	static constexpr la::Matrix<double, n_points, n_dofs> basis()
	{
		return la::make_matrix<n_points, n_dofs>([](auto quadr, auto dof) constexpr
		{
			// Map the interval [-1, 1], on which quadrature rules are defined,
			// into the f.e.reference interval [0, 1]: x -> (1 + x) / 2
			return Element::basis(dof, (1 + Quadr::points[quadr].coords[0]) / 2);
		});
	}

	static constexpr la::Matrix<double, n_points, n_dofs> basis_grad()
	{
		return la::make_matrix<n_points, n_dofs>([](auto quadr, auto dof) constexpr
		{
			// Map the interval [-1, 1], on which quadrature rules are defined,
			// into the f.e.reference interval [0, 1]: x -> (1 + x) / 2
			return Element::basis_grad(dof, (1 + Quadr::points[quadr].coords[0]) / 2);
		});
	}
};
} // namespace es_fe