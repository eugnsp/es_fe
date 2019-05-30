#pragma once
#include <es_fe/math/quadr.hpp>
#include <es_fe/types.hpp>

#include <es_la/dense.hpp>

namespace es_fe
{
namespace internal
{
template<class Element, class Quadr, class Grads, class Inv_tr_jacobian>
auto gradients(Grads& grads, const Inv_tr_jacobian& j)
{
	constexpr auto basis_grads = es_fe::Element_quadr<Element, Quadr>::basis_grad();
	for (es_fe::Local_index i = 0; i < grads.rows(); ++i)
		for (es_fe::Local_index r = 0; r < grads.cols(); ++r)
			grads(i, r) = j * basis_grads(i, r);
}
} // namespace internal

template<class Element, class Quadr>
auto gradients(double inv_j)
{
	es_la::Matrix<double, Quadr::size, Element::n_total_edge_dofs> grads;
	es_fe::internal::gradients<Element, Quadr>(grads, inv_j);
	return grads;
}

template<class Element, class Quadr>
auto gradients(const es_la::Matrix_2d& inv_tr_j)
{
	es_la::Matrix<es_la::Vector_2d, Quadr::size, Element::n_total_face_dofs> grads;
	es_fe::internal::gradients<Element, Quadr>(grads, inv_tr_j);
	return grads;
}
} // namespace es_fe
