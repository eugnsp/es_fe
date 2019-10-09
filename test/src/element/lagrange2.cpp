#include <esf/element/lagrange.hpp>
#include <esf/types.hpp>
#include <esl/dense.hpp>

#include <cassert>
#include <cmath>
#include <iostream>

using V = la::Vector_2d;
using I = esf::Local_index;

bool double_eq(double x, double y)
{
	return std::abs(x - y) < 1e-10;
}

template<class Element, I vertex_dofs, I edge_dofs, I face_dofs, I total_edge_dofs,
		 I total_face_dofs>
void ndofs_test()
{
	static_assert(Element::dim == 2);

	static_assert(Element::n_dofs(esf::Vertex_tag{}) == vertex_dofs);
	static_assert(Element::n_dofs(esf::Edge_tag{}) == edge_dofs);
	static_assert(Element::n_dofs(esf::Face_tag{}) == face_dofs);
	static_assert(Element::n_dofs(esf::Cell_tag{}) == face_dofs);

	static_assert(Element::n_total_dofs(esf::Edge_tag{}) == total_edge_dofs);
	static_assert(Element::n_total_dofs(esf::Face_tag{}) == total_face_dofs);
	static_assert(Element::n_total_dofs(esf::Cell_tag{}) == total_face_dofs);

	static_assert(Element::n_vertex_dofs == vertex_dofs);
	static_assert(Element::n_edge_dofs == edge_dofs);
	static_assert(Element::n_cell_dofs == face_dofs);

	static_assert(Element::n_total_edge_dofs == total_edge_dofs);
	static_assert(Element::n_total_face_dofs == total_face_dofs);
	static_assert(Element::n_total_cell_dofs == total_face_dofs);

	static_assert(Element::has_dofs(esf::Vertex_tag{}) == (vertex_dofs > 0));
	static_assert(Element::has_dofs(esf::Edge_tag{}) == (edge_dofs > 0));
	static_assert(Element::has_dofs(esf::Face_tag{}) == (face_dofs > 0));
	static_assert(Element::has_dofs(esf::Cell_tag{}) == (face_dofs > 0));

	static_assert(Element::has_vertex_dofs == (vertex_dofs > 0));
	static_assert(Element::has_edge_dofs == (edge_dofs > 0));
	static_assert(Element::has_face_dofs == (face_dofs > 0));
	static_assert(Element::has_cell_dofs == (face_dofs > 0));
}

template<class Element>
void basis_test()
{
	const V vertices[3] = {V({0, 0}), V({1, 0}), V({0, 1})};

	// Check values of basis functions at vertices
	for (unsigned int i = 0; i < Element::n_total_cell_dofs; ++i)
		for (unsigned int j = 0; j < 3; ++j)
			assert(double_eq(Element::basis(i, vertices[j]), i == j ? 1 : 0));
}

void basis_2_test()
{
	using Element = esf::Lagrange<2, 2>;
	const V edges[3] = {V({.5, 0}), V({.5, .5}), V({0, .5})};

	// Check values of basis functions at edges
	for (unsigned int i = 0; i < Element::n_total_cell_dofs; ++i)
		for (unsigned int j = 0; j < 3; ++j)
			assert(double_eq(Element::basis(i, edges[j]), i == j + 3 ? 1 : 0));
}

void basis_3_test()
{
	using Element = esf::Lagrange<3, 2>;
	const V edges[6] = {V({1 / 3., 0}),      V({2 / 3., 0}), V({2 / 3., 1 / 3.}),
						V({1 / 3., 2 / 3.}), V({0, 2 / 3.}), V({0, 1 / 3.})};

	// Check values of basis functions at edges
	for (unsigned int i = 0; i < Element::n_total_cell_dofs; ++i)
		for (unsigned int j = 0; j < 3 * Element::n_edge_dofs; ++j)
			assert(double_eq(Element::basis(i, edges[j]), i == j + 3 ? 1 : 0));

	// Check values of basis functions at the center
	const V center = V({1 / 3., 1 / 3.});
	for (unsigned int i = 0; i < Element::n_total_cell_dofs; ++i)
		assert(double_eq(Element::basis(i, center), i == 9 ? 1 : 0));
}

int main()
{
	ndofs_test<esf::Lagrange<1, 2>, 1, 0, 0, 2, 3>();
	ndofs_test<esf::Lagrange<2, 2>, 1, 1, 0, 3, 6>();
	ndofs_test<esf::Lagrange<3, 2>, 1, 2, 1, 4, 10>();
	ndofs_test<esf::Lagrange<4, 2>, 1, 3, 3, 5, 15>();

	basis_test<esf::Lagrange<1, 2>>();
	basis_test<esf::Lagrange<2, 2>>();
	basis_test<esf::Lagrange<3, 2>>();
	basis_test<esf::Lagrange<4, 2>>();

	basis_2_test();
	basis_3_test();

	std::cout << "OK.\n";
	return 0;
}
