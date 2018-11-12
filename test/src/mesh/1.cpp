#include "construct_mesh2.hpp"

#include <es_fe/mesh.hpp>

#include <cassert>
#include <iostream>

void construction_test(const es_fe::Mesh2& mesh)
{
	const auto err = mesh.check();
	if (err)
	{
		std::cout << err.message() << std::flush;
		assert(false);
	}
}

void basic_test(const es_fe::Mesh2& mesh)
{
	static_assert(mesh.dim == 2);	
	assert(!mesh.is_empty());

	std::cout << mesh.type_string() << std::endl;
	std::cout << mesh << std::endl;
}

int main()
{
	const auto mesh = construct_mesh();	

	int i = 0xABABAB;
	for (auto f = mesh.begin_face(); f != mesh.end_face(); ++f)
		++i;
		
	std::cout << i << std::endl;

	std::cout << "OK.\n";
	return 0;
}
