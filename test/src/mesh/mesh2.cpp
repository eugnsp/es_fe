#include <esf/types.hpp>
#include <esf/mesh/mesh2.hpp>

#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <vector>

void construction_test()
{
	using namespace es_fe::literals;

	std::vector<std::array<es_fe::Vertex_index, 3>> faces;
	faces.push_back({0_vi, 6_vi, 3_vi});
	faces.push_back({3_vi, 4_vi, 2_vi});
	faces.push_back({3_vi, 6_vi, 4_vi});
	faces.push_back({6_vi, 5_vi, 4_vi});
	faces.push_back({0_vi, 5_vi, 6_vi});
	faces.push_back({5_vi, 2_vi, 4_vi});
	faces.push_back({5_vi, 1_vi, 2_vi});
	faces.push_back({0_vi, 1_vi, 5_vi});
	faces.push_back({1_vi, 7_vi, 2_vi});
	std::sort(faces.begin(), faces.end());

	do
	{
		es_fe::Mesh2 mesh;
		mesh.add_vertex({0, 0});
		mesh.add_vertex({1, 0});
		mesh.add_vertex({1, 1});
		mesh.add_vertex({0, 1});
		mesh.add_vertex({.5, .6});
		mesh.add_vertex({.5, .4});
		mesh.add_vertex({.4, .5});
		mesh.add_vertex({2, .5});

		for (auto& face : faces)
			mesh.add_cell(face);

		if (const auto err = mesh.check(); err)
		{
			std::cout << err.message() << std::endl;
			assert(false);
		}
	} while (std::next_permutation(faces.begin(), faces.end()));
}

// void basic_test(const es_fe::Mesh2& mesh)
// {
// 	static_assert(mesh.dim == 2);
// 	assert(!mesh.is_empty());

// 	std::cout << mesh.type_string() << std::endl;
// 	std::cout << mesh << std::endl;
// }

int main()
{
	construction_test();
	// basic_test(mesh);

	std::cout << "OK.\n";
	return 0;
}
