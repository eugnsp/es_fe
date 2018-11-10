#include <es_fe/mesh.hpp>

#include <es_geom/algorithm.hpp>
#include <es_util/error.hpp>
#include <es_util/string.hpp>

#include <string>
#include <algorithm>
#include <limits>
#include <ostream>

namespace es_fe
{
// Performs some basic checks of mesh data structure consistency
es_util::Error Mesh2::check() const
{
	auto err = Halfedge_structure::check();
	if (err)
		return err;

	// Check for counter-clockwise order of vertices
	for (auto& face : this->faces())
		if (geom::area(face) < 0)
			err.append_ln("face #", **face, " has incorrect vertex order");

	return err;
}

// Returns human readable information about a mesh
void Mesh2::print(std::ostream& os) const
{
	os << type_string() << '\n'
	   << "Vertices: " << *n_vertices() << '\n'
	   << "Edges:    " << *n_edges() << '\n'
	   << "Faces:    " << *n_faces() << '\n'
	   << "Memory:   " << es_util::size_string(memory_size()) << std::endl;
}
} // namespace es_fe
