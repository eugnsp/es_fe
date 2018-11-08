#pragma once
#include <es_fe/element/linear_element.hpp>

TEST(linear_element, main)
{
	using E = es_fe::internal::Linear_element<1, 2>;
	
	EXPECT_EQ(E::dim, 1);
	
	EXPECT_EQ(E::n_dofs(es_fe::Vertex_tag{}), 1);
	EXPECT_EQ(E::n_dofs(es_fe::Edge_tag{}), 2);
	EXPECT_EQ(E::n_dofs(es_fe::Cell_tag{}), 2);

	EXPECT_EQ(E::n_total_dofs(es_fe::Edge_tag{}), 4);
	EXPECT_EQ(E::n_total_dofs(es_fe::Cell_tag{}), 4);

	EXPECT_EQ(E::n_vertex_dofs, 1);
	EXPECT_EQ(E::n_edge_dofs, 2);
	EXPECT_EQ(E::n_cell_dofs, 2);

	EXPECT_EQ(E::n_total_edge_dofs, 4);
	EXPECT_EQ(E::n_total_cell_dofs, 4);

	EXPECT_TRUE(E::has_dofs(es_fe::Vertex_tag{}));
	EXPECT_TRUE(E::has_dofs(es_fe::Edge_tag{}));
	EXPECT_TRUE(E::has_dofs(es_fe::Cell_tag{}));

	EXPECT_TRUE(E::has_vertex_dofs);
	EXPECT_TRUE(E::has_edge_dofs);
	EXPECT_TRUE(E::has_cell_dofs);
}
