#pragma once
#include <es_fe/element/triangular_element.hpp>

TEST(triangular_element, main)
{
	using E = es_fe::internal::Triangular_element<1, 2, 3>;
	
	EXPECT_EQ(E::dim, 2);
	
	EXPECT_EQ(E::n_dofs(es_fe::Vertex_tag{}), 1);
	EXPECT_EQ(E::n_dofs(es_fe::Edge_tag{}), 2);
	EXPECT_EQ(E::n_dofs(es_fe::Face_tag{}), 3);
	EXPECT_EQ(E::n_dofs(es_fe::Cell_tag{}), 3);

	EXPECT_EQ(E::n_total_dofs(es_fe::Edge_tag{}), 4);
	EXPECT_EQ(E::n_total_dofs(es_fe::Face_tag{}), 12);
	EXPECT_EQ(E::n_total_dofs(es_fe::Cell_tag{}), 12);

	EXPECT_EQ(E::n_vertex_dofs, 1);
	EXPECT_EQ(E::n_edge_dofs, 2);
	EXPECT_EQ(E::n_face_dofs, 3);
	EXPECT_EQ(E::n_cell_dofs, 3);

	EXPECT_EQ(E::n_total_edge_dofs, 4);
	EXPECT_EQ(E::n_total_face_dofs, 12);
	EXPECT_EQ(E::n_total_cell_dofs, 12);

	EXPECT_TRUE(E::has_dofs(es_fe::Vertex_tag{}));
	EXPECT_TRUE(E::has_dofs(es_fe::Edge_tag{}));
	EXPECT_TRUE(E::has_dofs(es_fe::Face_tag{}));
	EXPECT_TRUE(E::has_dofs(es_fe::Cell_tag{}));

	EXPECT_TRUE(E::has_vertex_dofs);
	EXPECT_TRUE(E::has_edge_dofs);
	EXPECT_TRUE(E::has_face_dofs);
	EXPECT_TRUE(E::has_cell_dofs);
}
