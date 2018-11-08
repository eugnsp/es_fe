#include <es_fe/types.hpp>
#include <es_fe/element/lagrange.hpp>
#include <es_fe/var.hpp>

#include <cassert>
#include <iostream>

using Element = es_fe::Lagrange<1, 2>;
constexpr es_fe::Local_index dim = 6;

struct Ess_bnd_cond
{
	Ess_bnd_cond(int tag) : tag(tag)
	{}

	static constexpr bool is_essential()
	{
		return true;
	}

	const int tag;
};

struct Non_ess_bnd_cond
{
	Non_ess_bnd_cond(int tag) : tag(tag)
	{}

	static constexpr bool is_essential()
	{
		return false;
	}

	const int tag;
};

template<class Element, class Var>
void test_base(Var& var)
{
	static_assert(var.space_dim == Element::dim);
	static_assert(var.n_bnd_conds == 2);
	
	var.set_name("test_name");
	assert(var.name() == "test_name");
	
	var.template set_bnd_cond<0>(123);
	var.template set_bnd_cond<1>(456);
	
	int tag1 = 0;
	var.for_each_ess_bnd_cond([&tag1](auto& bc) { tag1 += bc.tag; });
	assert(tag1 == 123);

	int tag2 = 0;
	var.for_each_non_ess_bnd_cond([&tag2](auto& bc) { tag2 += bc.tag; });
	assert(tag2 == 456);	
}

void test_base_var()
{
	es_fe::internal::Var_base<Element, Ess_bnd_cond, Non_ess_bnd_cond> var;
	test_base<Element>(var);
}

void test_static_var()
{
	es_fe::Var<Element, dim, Ess_bnd_cond, Non_ess_bnd_cond> var;
	test_base<Element>(var);

	static_assert(var.dim() == dim);
	
	static_assert(var.n_dofs(es_fe::Vertex_tag{}) == dim * Element::n_vertex_dofs);
	static_assert(var.n_dofs(es_fe::Edge_tag{}) == dim * Element::n_edge_dofs);
	static_assert(var.n_dofs(es_fe::Face_tag{}) == dim * Element::n_face_dofs);
	static_assert(var.n_dofs(es_fe::Cell_tag{}) == dim * Element::n_cell_dofs);

	static_assert(var.n_total_dofs(es_fe::Edge_tag{}) == dim * Element::n_total_edge_dofs);
	static_assert(var.n_total_dofs(es_fe::Face_tag{}) == dim * Element::n_total_face_dofs);
	static_assert(var.n_total_dofs(es_fe::Cell_tag{}) == dim * Element::n_total_cell_dofs);
}

void test_dynamic_var()
{
	es_fe::Var_x<Element, Ess_bnd_cond, Non_ess_bnd_cond> var;
	var.set_dim(dim);
	test_base<Element>(var);

	assert(var.dim() == dim);
	
	assert(var.n_dofs(es_fe::Vertex_tag{}) == dim * Element::n_vertex_dofs);
	assert(var.n_dofs(es_fe::Edge_tag{}) == dim * Element::n_edge_dofs);
	assert(var.n_dofs(es_fe::Face_tag{}) == dim * Element::n_face_dofs);
	assert(var.n_dofs(es_fe::Cell_tag{}) == dim * Element::n_cell_dofs);

	assert(var.n_total_dofs(es_fe::Edge_tag{}) == dim * Element::n_total_edge_dofs);
	assert(var.n_total_dofs(es_fe::Face_tag{}) == dim * Element::n_total_face_dofs);
	assert(var.n_total_dofs(es_fe::Cell_tag{}) == dim * Element::n_total_cell_dofs);
}

int main()
{
	test_base_var();
	test_static_var();
	test_dynamic_var();

	std::cout << "OK.\n";
	return 0;
}
