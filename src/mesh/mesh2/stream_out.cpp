#include <es_fe/mesh.hpp>

#include <es_util/string.hpp>

#include <string>
#include <algorithm>
#include <limits>
#include <ostream>

namespace es_fe
{
// Returns human readable information about a mesh
std::ostream& operator<<(std::ostream& os, const Mesh2& mesh)
{
	os << mesh.type_string() << '\n'
	   << "Vertices: " << *mesh.n_vertices() << '\n'
	   << "Edges:    " << *mesh.n_edges() << '\n'
	   << "Faces:    " << *mesh.n_faces() << '\n'
	   << "Memory:   " << es_util::size_string(mesh.memory_size()) << std::endl;

	return os;
}
} // namespace es_fe
