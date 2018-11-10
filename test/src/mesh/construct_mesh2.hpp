#pragma once
#include <es_fe/mesh.hpp>

es_fe::Mesh2 construct_mesh()
{
	es_fe::Mesh2 mesh;
	
	mesh.add_vertex({0, 0});
	mesh.add_vertex({1, 0});
	mesh.add_vertex({1, 1});
	mesh.add_vertex({0, 1});
	mesh.add_vertex({.5, 0});
	mesh.add_vertex({1, .5});
	mesh.add_vertex({.5, 1});
	mesh.add_vertex({0, .5});
	mesh.add_vertex({.5, .5});
	mesh.add_vertex({.25, .75});
	mesh.add_vertex({.75, .75});
	mesh.add_vertex({.25, .25});
	mesh.add_vertex({.75, .25});

	using namespace es_fe::literals;

	mesh.add_cell(9_vi, 8_vi, 6_vi);
	mesh.add_cell(7_vi, 11_vi, 9_vi);
	mesh.add_cell(8_vi, 10_vi, 6_vi);
	mesh.add_cell(11_vi, 8_vi, 9_vi);
	mesh.add_cell(9_vi, 6_vi, 3_vi);
	mesh.add_cell(0_vi, 11_vi, 7_vi);
	mesh.add_cell(10_vi, 5_vi, 2_vi);
	mesh.add_cell(4_vi, 1_vi, 12_vi);
	mesh.add_cell(8_vi, 5_vi, 10_vi);
	mesh.add_cell(4_vi, 12_vi, 8_vi);
	mesh.add_cell(11_vi, 4_vi, 8_vi);
	mesh.add_cell(8_vi, 12_vi, 5_vi);
	mesh.add_cell(0_vi, 4_vi, 11_vi);
	mesh.add_cell(12_vi, 1_vi, 5_vi);
	mesh.add_cell(7_vi, 9_vi, 3_vi);
	mesh.add_cell(10_vi, 2_vi, 6_vi);
	
	return mesh;
}
