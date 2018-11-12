#include <es_fe/mesh/io.hpp>
#include <es_fe/mesh/tensor_mesh.hpp>

#include <iostream>
#include <initializer_list>

#include <vector>
#include <unordered_map>

template<class Circulator>
auto enum_element_indices(Circulator circ)
{
	std::vector<typename Circulator::Element_index> indices;

	const auto begin = circ;
	do
		indices.push_back(**circ);
	while (++circ != begin);

	return indices;
}

template<class List>
bool equal_indices(const List& vi1, std::initializer_list<es_fe::Index> vi2)
{
	return std::equal(vi1.begin(), vi1.end(), vi2.begin(), vi2.end(),
		[](auto i1, es_fe::Index i2) { return *i1 == i2; });
}

int main__(int argc, char** argv)
{
	geom::Linear_grid lines_x, lines_y;

 	lines_x.add_tick(0);
 	lines_x.add_tick(2, 10);
 
 	lines_y.add_tick(0);
 	lines_y.add_tick(2, 10);

	es_fe::Tri_tensor_mesh mesh(lines_x.grid(), lines_y.grid());

	//std::cout << mesh << '\n';

	auto eb = mesh.begin_edge();
	auto ee = mesh.end_edge();

	for (; eb != ee; ++eb)
		std::cout << ***eb << '\n';

	//////////////////////////////////////////////////////////////////////////
	/** Vertex view */
	{
		auto v1 = mesh.begin_vertex() + 3;		
		assert(***v1 == 3);
		assert(v1->vertex() == geom::Point(0, 5));
		assert(v1->is_boundary());
		assert(v1->n_edges() == 4);
		assert(v1->n_faces() == 3);

		auto vertices1 = enum_element_indices(v1->vertex_circ());
		assert(equal_indices(vertices1, {6, 7, 4, 0}));		
		auto hes_in1 = enum_element_indices(v1->halfedge_in_circ());
		assert(equal_indices(hes_in1, {24, 20, 6, 9}));
		auto hes_out1 = enum_element_indices(v1->halfedge_out_circ());
		assert(equal_indices(hes_out1, {25, 21, 7, 8}));
		auto edges1 = enum_element_indices(v1->edge_circ());
		assert(equal_indices(edges1, {12, 10, 3, 4}));

		auto v2 = mesh.begin_vertex() + 4;
		assert(***v2 == 4);
		assert(v2->vertex() == geom::Point(5, 5));
		assert(!v2->is_boundary());
		assert(v2->n_edges() == 6);
		assert(v2->n_faces() == 6);

		auto vertices2 = enum_element_indices(v2->vertex_circ());
		assert(equal_indices(vertices2, {8, 5, 1, 0, 3, 7}));
		auto hes_in2 = enum_element_indices(v2->halfedge_in_circ());
		assert(equal_indices(hes_in2, {28, 16, 2, 5, 7, 19}));
		auto hes_out2 = enum_element_indices(v2->halfedge_out_circ());
		assert(equal_indices(hes_out2, {29, 17, 3, 4, 6, 18}));
		auto edges2 = enum_element_indices(v2->edge_circ());
		assert(equal_indices(edges2, {14, 8, 1, 2, 3, 9}));

		auto v3 = mesh.begin_vertex() + 8;
		assert(***v3 == 8);
		assert(v3->vertex() == geom::Point(10, 10));
		assert(v3->is_boundary());
		assert(v3->n_edges() == 3);
		assert(v3->n_faces() == 2);

		auto vertices3 = enum_element_indices(v3->vertex_circ());
		assert(equal_indices(vertices3, {5, 4, 7}));
		auto hes_in3 = enum_element_indices(v3->halfedge_in_circ());
		assert(equal_indices(hes_in3, {26, 29, 31}));
		auto hes_out3 = enum_element_indices(v3->halfedge_out_circ());
		assert(equal_indices(hes_out3, {27, 28, 30}));
		auto edges3 = enum_element_indices(v3->edge_circ());
		assert(equal_indices(edges3, {13, 14, 15}));
	}

	//////////////////////////////////////////////////////////////////////////
	/** Face view */
	{
		es_fe::Element_view<es_fe::Face_tag, es_fe::Mesh2>::Vertex_indices i_vertices;
		es_fe::Element_view<es_fe::Face_tag, es_fe::Mesh2>::Edge_with_dir_indices i_edges;

		es_fe::Vertex_index vi{2};

		auto f1 = mesh.begin_face();
		assert(***f1 == 0);
		assert(f1->is_boundary());

		f1->get_indices(i_vertices);
		assert(equal_indices(i_vertices, {0, 1, 4}));
		f1->get_indices(i_vertices, i_edges);
		assert(equal_indices(i_vertices, {0, 1, 4}));
		assert(*i_edges[0].first == 2 && *i_edges[1].first == 0 && *i_edges[2].first == 1);
		assert(i_edges[0].second && i_edges[1].second && i_edges[2].second);

		auto vertices1 = enum_element_indices(f1->vertex_circ());
		assert(equal_indices(vertices1, {0, 1, 4}));
		auto hes1 = enum_element_indices(f1->halfedge_circ());
		assert(equal_indices(hes1, {4, 0, 2}));
		auto edges1 = enum_element_indices(f1->edge_circ());
		assert(equal_indices(edges1, {2, 0, 1}));
		auto faces1 = enum_element_indices(f1->face_circ());
		assert(equal_indices(faces1, {1, 3}));
		
		auto f2 = mesh.begin_face() + 3;
		assert(***f2 == 3);
		assert(!f2->is_boundary());

		auto vertices2 = enum_element_indices(f2->vertex_circ());
		assert(equal_indices(vertices2, {1, 5, 4}));
		auto he2 = enum_element_indices(f2->halfedge_circ());
		assert(equal_indices(he2, {3, 15, 16}));
		auto edges2 = enum_element_indices(f2->edge_circ());
		assert(equal_indices(edges2, {1, 7, 8}));
		auto faces2 = enum_element_indices(f2->face_circ());
		assert(equal_indices(faces2, {0, 2, 6}));

		f2->get_indices(i_vertices);
		assert(equal_indices(i_vertices, {1, 5, 4}));
		f2->get_indices(i_vertices, i_edges);
		assert(equal_indices(i_vertices, {1, 5, 4}));
		assert(*i_edges[0].first == 1 && *i_edges[1].first == 7 && *i_edges[2].first == 8);
		assert(!i_edges[0].second && !i_edges[1].second && i_edges[2].second);
	}

// 	do 
// 	{
// 		std::cout << ***vc1 << '\n';
// 		std::cout << vc1->is_boundary() << '\n';
// 	}
// 	while (++vc1 != vc2);

	return 0;
}
