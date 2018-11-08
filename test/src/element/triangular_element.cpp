#include <es_fe/types.hpp>
#include <es_fe/element/triangular_element.hpp>

#include <cassert>
#include <iostream>

using Element = es_fe::internal::Triangular_element<1, 2, 3>;

void triangular_element()
{	
	static_assert(Element::dim == 2);
	
	static_assert(Element::n_dofs(es_fe::Vertex_tag{}) == 1);
	static_assert(Element::n_dofs(es_fe::Edge_tag{}) == 2);
	static_assert(Element::n_dofs(es_fe::Face_tag{}) == 3);
	static_assert(Element::n_dofs(es_fe::Cell_tag{}) == 3);

	static_assert(Element::n_total_dofs(es_fe::Edge_tag{}) == 4);
	static_assert(Element::n_total_dofs(es_fe::Face_tag{}) == 12);
	static_assert(Element::n_total_dofs(es_fe::Cell_tag{}) == 12);

	static_assert(Element::n_vertex_dofs == 1);
	static_assert(Element::n_edge_dofs == 2);
	static_assert(Element::n_face_dofs == 3);
	static_assert(Element::n_cell_dofs == 3);

	static_assert(Element::n_total_edge_dofs == 4);
	static_assert(Element::n_total_face_dofs == 12);
	static_assert(Element::n_total_cell_dofs == 12);

	static_assert(Element::has_dofs(es_fe::Vertex_tag{}));
	static_assert(Element::has_dofs(es_fe::Edge_tag{}));
	static_assert(Element::has_dofs(es_fe::Face_tag{}));
	static_assert(Element::has_dofs(es_fe::Cell_tag{}));

	static_assert(Element::has_vertex_dofs);
	static_assert(Element::has_edge_dofs);
	static_assert(Element::has_face_dofs);
	static_assert(Element::has_cell_dofs);
}

int main()
{
	triangular_element();

	std::cout << "OK.\n";
	return 0;
}
